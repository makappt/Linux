#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

class TaskQueue
{
public:
    TaskQueue(int maxsize) : _maxSize(maxsize) {}
    static void *producer(void *arg);
    static void *consumer(void *arg);
    static TaskQueue &getTask();

    int _maxSize;
    static queue<int> taskQueues;
    pthread_cond_t producerCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t consumerCond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

private:
    TaskQueue &operator=(const TaskQueue &) = delete;
    TaskQueue(const TaskQueue &) = delete;
};

queue<int> TaskQueue::taskQueues;

TaskQueue &TaskQueue::getTask()
{
    static TaskQueue task(10);
    return task;
}

void *TaskQueue::producer(void *arg)
{
    TaskQueue &taskQueue = getTask();
    while (true)
    {
        pthread_mutex_lock(&taskQueue.mutex);
        while (taskQueue.taskQueues.size() == taskQueue._maxSize)
        {
            pthread_cond_wait(&taskQueue.producerCond, &taskQueue.mutex);
        }

        int num = rand() % 100 + 1;
        taskQueue.taskQueues.push(num);
        cout << "Produced: " << num << endl;

        pthread_cond_signal(&taskQueue.consumerCond);
        pthread_mutex_unlock(&taskQueue.mutex);
    }
    return nullptr;
}

void *TaskQueue::consumer(void *arg)
{
    TaskQueue &taskQueue = getTask();
    while (true)
    {
        pthread_mutex_lock(&taskQueue.mutex);
        while (taskQueue.taskQueues.empty())
        {
            pthread_cond_wait(&taskQueue.consumerCond, &taskQueue.mutex);
        }

        int num = taskQueue.taskQueues.front();
        taskQueue.taskQueues.pop();
        cout << "Consumed: " << num << endl;

        pthread_cond_signal(&taskQueue.producerCond);
        pthread_mutex_unlock(&taskQueue.mutex);
    }
    return nullptr;
}

int main()
{
    srand(time(NULL));
    pthread_t producers[3], consumers[3];

    // 创建生产者线程
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&producers[i], NULL, TaskQueue::producer, NULL);
    }

    // 创建消费者线程
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&consumers[i], NULL, TaskQueue::consumer, NULL);
    }

    // 等待线程结束
    for (int i = 0; i < 3; i++)
    {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    return 0;
}
