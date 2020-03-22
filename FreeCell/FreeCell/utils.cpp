#include "utils.h"

using namespace std;

random_device g_rd;
#if	1
mt19937 g_mt(g_rd());
#else
mt19937 g_mt(10);
#endif
