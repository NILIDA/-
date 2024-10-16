#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <cassert>

using namespace std;

// Глобальные переменные
mutex mtx;
condition_variable cv;
bool music_playing;
bool game_over;

// Функция для моделирования действий ребенка
void child(int id, vector<bool>& chairs, int& remaining_children) {
    unique_lock<mutex> lock(mtx);
    while (!game_over) {
        cv.wait(lock, [] { return !music_playing; }); // Ждём, пока музыка остановится

        // Пытаемся найти свободный стул
        bool found_chair = false;
        for (int i = 0; i < chairs.size(); ++i) {
            if (!chairs[i]) {
                chairs[i] = true; // Занимаем стул
                found_chair = true;
                cout << "Child " << id << " sat on chair " << i << endl;
                break;
            }
        }

        if (!found_chair) {
            cout << "Child " << id << " did not find a chair and is leaving." << endl;
            --remaining_children;

            // Если остался один ребенок, заканчиваем игру
            if (remaining_children == 1) {
                game_over = true;
                cv.notify_all(); // Уведомляем остальные потоки о завершении
            }

            return; // Ребенок уходит
        }

        cv.wait(lock, [] { return music_playing || game_over; }); // Ждем, пока музыка снова начнет играть или игра завершится
    }

    // Проверка на победителя, если игра ещё не завершена
    if (remaining_children == 1 && !game_over) {
        cout << "Child " << id << " is the winner!" << endl;
    }
}

// Функция для управления музыкой
void play_music(int& remaining_children, vector<bool>& chairs) {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(1, 3);

    while (remaining_children > 1) {
        // Начинаем играть музыку
        {
            lock_guard<mutex> lock(mtx);
            music_playing = true;
            cout << "Music is playing..." << endl;
        }
        cv.notify_all();
        this_thread::sleep_for(chrono::seconds(dist(rng))); // Музыка играет случайное время

        // Останавливаем музыку
        {
            lock_guard<mutex> lock(mtx);
            music_playing = false;
            cout << "Music stopped!" << endl;
        }
        cv.notify_all();
        this_thread::sleep_for(chrono::seconds(2)); // Время для того, чтобы дети сели

        // Уменьшаем количество стульев для следующего раунда
        if (remaining_children > 1) {
            chairs.pop_back(); // Убираем один стул
            cout << "One chair is removed. Remaining chairs: " << chairs.size() << endl;
        }

        // Освобождаем стулья
        chairs.assign(chairs.size(), false);
    }

    // Игра окончена, у нас есть победитель
    {
        lock_guard<mutex> lock(mtx);
        game_over = true;
        cout << "We have a winner!" << endl;
    }
    cv.notify_all();
}

// Функция для сброса состояния перед каждым тестом
void reset_game_state(int N) {
    music_playing = true;
    game_over = false;
}

// Функция для тестирования игры с разным количеством детей
void test_game_with_children(int num_children) {
    int remaining_children = num_children;
    reset_game_state(num_children);
    vector<bool> chairs(num_children - 1, false); // Количество стульев на один меньше, чем детей

    // Создаем потоки для детей
    vector<thread> children;
    for (int i = 0; i < num_children; ++i) {
        children.emplace_back(child, i + 1, ref(chairs), ref(remaining_children));
    }

    // Запускаем поток музыки
    thread music_thread(play_music, ref(remaining_children), ref(chairs));

    // Ожидаем завершения всех потоков детей
    for (auto& t : children) {
        t.join();
    }

    // Ожидаем завершения потока музыки
    music_thread.join();

    // Проверяем, что остался только один победитель
    assert(remaining_children == 1);
    cout << "Test with " << num_children << " children passed!" << endl;
}

int main() {
    // Запуск тестов с разным количеством детей
    test_game_with_children(3);
    test_game_with_children(4);
    test_game_with_children(5);
    test_game_with_children(6);

    cout << "All tests passed!" << endl;
    return 0;
}
