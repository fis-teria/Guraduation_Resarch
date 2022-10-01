small: small.cpp
	g++ -o small small.cpp `pkg-config opencv --cflags --libs`

match: match.cpp
	g++ -o match match.cpp `pkg-config opencv --cflags --libs`
