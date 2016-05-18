#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* fib(void* p)
{
  int* x = (int*) p;
  int i, e;
  int v[2] = {0, 1};
  int t;

  e = *x;
  for (i = 0; i < e; ++i) {
    x[i] = v[0];
    t = v[0] + v[1];
    v[0] = v[1];
    v[1] = t;
  }
  return 0;
}

int main()
{
  int* x;
  int cnt;
  pthread_t pThread;
  void* r;

  scanf("%d", &cnt);
  x = (int*) malloc(cnt * sizeof(int));
  x[0] = cnt;

  if (pthread_create(&pThread, 0, &fib, x) == 0 ) {
    int i;
    pthread_join(pThread, &r);
    for (i = 0; i < cnt; ++i) {
      if (i % 10 == 0)
        putchar('\n');
      printf("%d\t", x[i]);
    }
    putchar('\n');
  }

  free(x);
}

