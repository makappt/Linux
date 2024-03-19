// raise 自己给自己发信号 raise(signal) ==  kill(getpid(),signal)
// abort 自己给自己发异常终止信号SIGABRT,终止并产生core信号