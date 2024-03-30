// 信号量的传入参数应是共享资源的最大数量，而不是控制访问的线程数，信号量通过加减共享资源数量实现什么时候该生产什么时候该消费，什么时候阻塞
// 如果信号量减为0时阻塞线程，如果信号量不为0时能够让线程继续执行，但需要加锁防止数据读写混乱
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

using namespace std;

class TaskQueue
{
public:
    TaskQueue(int maxsize) : _maxSize(maxsize)
    {
        sem_init(&availableSpaces, 0, _maxSize); // 初始化为队列最大大小，表示初始时所有槽位都是空的
        sem_init(&productsAvailable, 0, 0);      // 初始时没有已填充的槽位
        pthread_mutex_init(&mutex, NULL);        // 初始化互斥锁
    }
    ~TaskQueue()
    {
        sem_destroy(&availableSpaces);
        sem_destroy(&productsAvailable);
        pthread_mutex_destroy(&mutex);
    }
    static void *producer(void *arg);
    static void *consumer(void *arg);
    static TaskQueue &getTask();

    int _maxSize;
    static pthread_mutex_t mutex;
    static queue<int> taskQueues;
    static sem_t availableSpaces;   // 空槽位的信号量
    static sem_t productsAvailable; // 已填充槽位的信号量

private:
    TaskQueue(const TaskQueue &) = delete;
    TaskQueue &operator=(const TaskQueue &) = delete;
};

queue<int> TaskQueue::taskQueues;
sem_t TaskQueue::availableSpaces;
sem_t TaskQueue::productsAvailable;
pthread_mutex_t TaskQueue::mutex;

TaskQueue &TaskQueue::getTask()
{
    static TaskQueue task(10); // 假设队列最大大小为10
    return task;
}

void *TaskQueue::producer(void *arg)
{
    TaskQueue &taskQueue = getTask();
    while (true)
    {
        sem_wait(&taskQueue.availableSpaces); // 减少空槽数，即减少能够继续生产的空间
        // 生产操作
        int num = rand() % 100 + 1;
        pthread_mutex_lock(&mutex); // 防止输出混乱
        taskQueue.taskQueues.push(num);
        cout << "Produced: " << num << endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&taskQueue.productsAvailable); // 增加产品数，如果产品数满了，那么空槽数就为0，不能继续生产
    }
    return nullptr;
}

void *TaskQueue::consumer(void *arg)
{
    TaskQueue &taskQueue = getTask();
    while (true)
    {
        sem_wait(&taskQueue.productsAvailable); // 减少产品数，增加了空槽数，增加了能生产的空间
        // 消费操作
        pthread_mutex_lock(&mutex);
        int num = taskQueue.taskQueues.front();
        taskQueue.taskQueues.pop();
        cout << "Consumed: " << num << endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&taskQueue.availableSpaces); // 增加空槽数，减少了产品数，如果产品数为0，不能继续消费
    }
    return nullptr;
}

int main()
{
    srand(time(nullptr));
    pthread_t producers[3], consumers[3];

    // 创建生产者线程
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&producers[i], nullptr, TaskQueue::producer, nullptr);
    }

    // 创建消费者线程
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&consumers[i], nullptr, TaskQueue::consumer, nullptr);
    }

    // 等待线程结束
    for (int i = 0; i < 3; i++)
    {
        pthread_join(producers[i], nullptr);
        pthread_join(consumers[i], nullptr);
    }

    return 0;
}
