# libevent2 tcp server
## linux  
cd ./src  

gcc main.c -o sdemo -L/usr/local/libevent/lib -levent_core -levent_pthreads

./sdemo



测试  
echo "111111111111111113333333333333333" | netcat 127.0.0.1 9999   
echo "111111111111111113333333333333333" | netcat 127.0.0.1 9998   

    
        

学习中......