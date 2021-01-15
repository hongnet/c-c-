a.out:main.cpp libhttpSDK.so libMyUtils.so
	g++ main.cpp -g -lhttpSDK -lMyUtils -lpthread -std=c++11 -Wl,-rpath=./ -I. -L. -g -o a.out
libhttpSDK.so:TcpServerSrc.cpp
	g++ TcpServerSrc.cpp -std=c++11 -Wl,-rpath=./ -g -I. -L. -lpthread -fPIC -shared -o libhttpSDK.so
drogon.out:dragonTest.cpp 
	g++ dragonTest.cpp -g -lhttpSDK -lpthread -std=c++11 -Wl,-rpath=./ -I. -L. -g -o drogon
libMyUtils.so:Utils.cpp
	g++ Utils.cpp -std=c++11 -Wl,-rpath=./ -g -I. -L.  -fPIC -shared -o libMyUtils.so
clean:
	rm -rf libhttpSDK.so a.out

