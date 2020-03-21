#include "utils.h"

using namespace std;

random_device g_rd;
#if	0
mt19937 g_mt(g_rd());
#else
mt19937 g_mt(2);
#endif
