// pthread_cond_timedwait() 设定绝对时间，计时元年为1970.1.1 00.00.00
/*
    使用方法：先获取当前时间 time_t cur = time(nullptr);
             然后使用       struct timespec t;
                           t.tv_sec = cur + 1;
                           pthread_cond_timedwait(&cond,&mutex,&t);
*/