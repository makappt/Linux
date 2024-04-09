/*
结合生产者消费者信号量模型，揣摩sem_timedwait函数作用。编程实现，一个线程读用户输入，
另一个线程打印“hello world”。如果用户无输入，则每隔5秒向屏幕打印一个“hello world”；
如果用户有输入，立刻打印“hello world”到屏幕。
*/