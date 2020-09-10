#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAM_SIZE 100

typedef struct {
  int R;
  int A;
  int B;
  unsigned int PC;
  int ram[RAM_SIZE];
} VM;

VM *new_VM() {
  VM *v = malloc(sizeof(VM));
  v->R = 0;
  v->A = 0;
  v->B = 0;
  v->PC = 0;
  for (int i = 0; i < RAM_SIZE; i++)
    v->ram[i] = 0;
  return v;
}

void load(VM *v, int *prog, unsigned int size) {
  memcpy(v->ram, prog, size * sizeof(int));
}

void input(VM *v, int value) { v->ram[RAM_SIZE - 1] = value; }

void output(VM *v) { printf("out => %d\n", v->ram[RAM_SIZE - 1]); }

void dump(VM *v) {
  for (int i = 0; i < RAM_SIZE; i++) {
    printf("=> %d\n", v->ram[i]);
  }
}

void m99_cycle(VM *v) {
  int opp = v->ram[v->PC];
  int xy_opp = opp % 100;
  int y_opp = xy_opp % 10;
  int x_opp = (xy_opp - y_opp) / 10;
  int opc = opp / 100;
  int *src;
  int *dst;

  if (v->PC >= RAM_SIZE) {
    exit(0);
  }

  switch (opc) {
  case 0:
    // STR
    v->ram[xy_opp] = v->R;
    if (xy_opp == RAM_SIZE - 1) {
      output(v);
    }
    break;
  case 1:
    // LDA
    if (xy_opp == RAM_SIZE - 1) {
      printf("Input $ ");
      fflush(stdout);
      scanf("%d", &(v->A));
    } else {
      v->A = v->ram[xy_opp];
    }
    break;
  case 2:
    // LDB
    if (xy_opp == RAM_SIZE - 1) {
      printf("Input $ ");
      fflush(stdout);
      scanf("%d", &(v->A));
    } else {
      v->B = v->ram[xy_opp];
    }
    break;
  case 3:
    // MOV
    src = (x_opp == 0 ? &(v->R) : (x_opp == 1 ? &(v->A) : &(v->B)));
    dst = (y_opp == 0 ? &(v->R) : (y_opp == 1 ? &(v->A) : &(v->B)));
    *dst = *src;
    break;
  case 4:
    if (y_opp) {
      // SUB
      v->R = v->A - v->B;
    } else {
      // ADD
      v->R = v->A + v->B;
    }
    break;
  case 5:
    // JMP
    v->PC = xy_opp - 1;
    break;
  case 6:
    // JPP
    if (v->R > 0) {
      v->PC = xy_opp - 1;
    }
    break;
  case 7:
    // JEQ
    if (v->R == xy_opp) {
      v->PC += 1;
    }
    break;
  case 8:
    // JEQ
    if (v->R != xy_opp) {
      v->PC += 1;
    }
    break;
  }
  v->PC += 1;
}

int main(int argc, char const *argv[]) {
  int prog_1[] = {110, 211, 401, 607, 310, 99,  599, 320, 99,  599, 42,
                  10,  0,   199, 310, 10,  199, 310, 11,  500, 0,   0,
                  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                  0,   0,   0,   0,   0,   0,   0,   199, 310, 59,  299,
                  320, 60,  162, 259, 400, 62,  160, 261, 401, 60,  646,
                  162, 310, 99,  599, 3,   2,   1,   0};

  VM *v = new_VM();
  load(v, prog_1, 63);
  v->PC = 40;
  while (1) {
    // printf("R: %d\nA: %d\nB: %d\nPC: %d\n\n", v->R, v->A, v->B, v->PC);
    m99_cycle(v);
  }
  return 0;
}
