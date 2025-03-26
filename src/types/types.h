#ifndef TYPES_H
#define TYPES_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/coroutine2/all.hpp>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
using GraphCoroutine = boost::coroutines2::coroutine<Graph&>;

#endif