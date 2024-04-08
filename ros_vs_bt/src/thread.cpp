#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

class Move
{
public:
    Move()
    {
        state = 0;
        cout << "[Move]: ACTIVE" << endl;
    }

    void update()

    {
        state++;
        cout << "[Move]: state -> " << state << endl;
    }

    int get_state()
    {
        return state;
    }

private:
    int state;
};

class BT
{
public:
    BT(Move *move_ptr) : move(move_ptr)
    {
        work = 0;
        cout << "[BT]: ACTIVE" << endl;
    }

    void update()
    {
        work += 2;
        cout << "[BT]: work -> " << work << endl;
        cout << "[BT]: state -> " << move->get_state() << endl;
    }

private:
    Move *move;
    int work;
};

int main(int argc, char **argv)
{
    Move move;
    BT bt(&move);

    std::chrono::seconds delay(1);

    std::thread move_thread([&move, &delay]()
                            {
        while (true) {
            move.update();
            std::this_thread::sleep_for(delay);
        } });

    std::thread bt_thread([&bt, &delay]()
                          {
        while (true) {
            bt.update();
            std::this_thread::sleep_for(delay);
        } });

    move_thread.join();
    bt_thread.join();
    return 0;
}