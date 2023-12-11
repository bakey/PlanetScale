all:
	g++ main.cc filter.cc corpus_manager.cc searcher.cc \
	-I contrib/simhash/deps/ \
	-I contrib/simhash/include \
	-I contrib/glog/build \
	-I contrib/uchardet/src \
	-I contrib/json/include \
	-I contrib/cppjieba/include \
	-L contrib/glog/build \
	-L contrib/uchardet/build/src \
	-lglog \
	-luchardet \
	-o planet_scale -std=c++14
