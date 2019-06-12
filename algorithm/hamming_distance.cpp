#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <random>
#include <queue>
#include <sys/time.h>
#include <stdint.h>
using namespace std;
//#define TEST_SIZE 100000000
#define TEST_SIZE 1000000

uint64_t pop_count_base(uint64_t n) {
  uint8_t res = 0;
  while (n) {
    res += n & 1;
    n = n >> 1;
  }
  return res;
}

uint64_t pop_count_asm(const uint64_t n) {
  __asm__ __volatile__ (
      "movq $0, %rax\n\t" // uint8_t res = 0;
      ".pop_count_asm_BEGIN_WHILE:\n\t"   // while (n) {
      "cmpq $0, %rdi\n\t" // while (n)
      "je .pop_count_asm_END_WHILE\n\t"  
      "movq %rdi, %rbx\n\t" 
      "andq $1, %rbx\n\t" // n & 1
      "addq %rbx, %rax\n\t" // res += n & 1
      "shrq $1, %rdi\n\t" // n >> 1
      "jmp .pop_count_asm_BEGIN_WHILE\n\t"
      ".pop_count_asm_END_WHILE:\n\t"   //
      // return res;
      );
}

uint64_t popcnt_buildin(uint64_t n) {
    return __builtin_popcountll(n);
}

uint64_t popcnt_sse_asm(uint64_t n) {
  __asm__ __volatile__ ("popcntq %rdi, %rax\n\t");
}

class Algorithm {
  public:
    ~Algorithm() {}
    virtual void init() {}
    virtual std::string getName() {
      return "Undefined Algorithm Name.";
    }
    virtual uint64_t cal(const uint64_t* p, const uint64_t* q, const uint64_t size) = 0;
};

class AlgorithmBench {
  public:
    void init() {
      startTimer();
      vector = new uint64_t[size];
      default_random_engine e;

      for(int i = 0; i < size;i++ ) {
        uint64_t c;
        f >> vector[i];
      }
      stopTimer();
      getInterval("load sample cost:");
      f.close();
    }
    void setSize(uint64_t size) { this->size = size;};
    void push_back(Algorithm* algorithm) { _algorithm_vector.push_back(algorithm);}
    
    void start() {
      for (std::vector<Algorithm*>::iterator iter = _algorithm_vector.begin();
          iter != _algorithm_vector.end();
          ++iter) {
        Algorithm* ptr = *iter;
        ptr->init();
        startTimer();
        for (int i = 0; i < size - 3; ++i) {
          ptr->cal(vector + i, vector + i + 1, 4);
        }
        stopTimer();
        getInterval(ptr->getName() + " cost:");
      }
    }

    void startTimer() {
      gettimeofday(&tv,NULL);
      start_timer = 1000000 * tv.tv_sec + tv.tv_usec;
    }
    void stopTimer() {
      gettimeofday(&tv,NULL);
      end_timer = 1000000 * tv.tv_sec + tv.tv_usec;
    }
    void getInterval(std::string prefix) {
      std::cout<<std::left<<setw(40) << prefix 
        << std::right << end_timer - start_timer<<endl;
    }

  private:
    uint64_t size;
    uint64_t *vector;
    timeval tv;
    uint64_t start_timer;
    uint64_t end_timer;
    std::vector<Algorithm*> _algorithm_vector;
};

class HammingDistanceBase : public Algorithm {
  public:
    std::string getName() {
      return "HammingDistanceBase";
    }

    uint64_t cal(const uint64_t* p, const uint64_t* q, const uint64_t size) {
      uint64_t res = 0;
      for (uint64_t i = 0; i < size; ++i) {
        uint64_t r = (*(p + i)) ^ (*(q + i));
        while (r) {
          res += r & 1;
          r = r >> 1;
        }
      }
      return res;
    }
};

uint64_t hamming_distance_base_asm(const uint64_t* p, const uint64_t* q, const uint64_t size) {
      __asm__ __volatile__ (
          "movq $0, %r10\n\t" // res = 0
          "movq $0, %r11\n\t" // i = 0
          "movq %rdi, %r12\n\t" // %r12 -> (p + i) 
          "movq %rsi, %r13\n\t" // %r13 -> (q + i)
          ".BEGIN_FOR:\n\t"
          "cmpq %r11, %rdx\n\t"  // while ( i < size) // 0 < size - i
          "jle .END_FOR\n\t"
          "movq (%r12), %rax\n\t"  // (*(p + i)) ^ (*(q + i));
          "movq (%r13), %rbx\n\t"  // (*(p + i)) ^ (*(q + i));
          "xorq %rbx, %rax\n\t"    // (*(p + i)) ^ (*(q + i));  %rax-> r
          ".BEGIN_WHILE:\n\t"
          "cmpq $0, %rax\n\t" // while (r)
          "je .END_WHILE\n\t"
          "movq %rax, %rbx\n\t" 
          "andq $1, %rbx\n\t" // r & 1
          "addq %rbx, %r10\n\t" // res += r & 1
          "shrq $1, %rax\n\t" // r >> 1
          "jmp .BEGIN_WHILE\n\t"
          ".END_WHILE:\n\t"
          "incq %r11\n\t"         // ++i;
          "addq $8, %r12\n\t"    // p + i
          "addq $8, %r13\n\t"    // q + i
          "jmp .BEGIN_FOR\n\t"
          ".END_FOR:\n\t"
          "movq %r10, %rax\n\t" // return res;
          ".END_FUN:\n\t" 
        );
}

class HammingDistanceBaseAsm : public Algorithm {
  public:
    std::string getName() { return "HammingDistanceBaseAsm"; }

    uint64_t cal(const uint64_t* p, const uint64_t* q, const uint64_t size) {
      return hamming_distance_base_asm(p, q, size);
    }
};
class HammingDistanceBuildin : public Algorithm {
  public:
    std::string getName() {
      return "HammingDistanceBuildin";
    }
    uint64_t cal(const uint64_t* p, const uint64_t* q, const uint64_t size) {
      uint64_t res = 0;
      for (uint64_t i = 0; i < size; ++i) {
        uint64_t r = (*(p + i)) ^ (*(q + i));
        res += __builtin_popcountll(r);
      }
      return res;
    }
};

class HammingDistanceTable8Bit : public Algorithm {
  public:
    std::string getName() {
      return "HammingDistanceTable8Bit";
    }
    void init() {
      pop_count_table_ptr = NULL;
      pop_count_table_8bit_init(&pop_count_table_ptr);
    }
    uint64_t cal(const uint64_t* p, const uint64_t* q, const uint64_t size) {
      uint64_t res = 0;
      for (uint64_t i = 0; i < size; ++i)
      {
        uint64_t r = (*(p + i)) ^ (*(q + i));
        res += pop_count_table_8bit(r);
      }
      return res;
    }
  private:
    uint8_t *pop_count_table_ptr; 
    void pop_count_table_8bit_init(uint8_t **pop_count_table_ptr) {
      *pop_count_table_ptr = new uint8_t[256];
      for (int i = 0; i < 256; ++i) {
        (*pop_count_table_ptr)[i] = __builtin_popcount(i);
      }  
    }
    
    uint64_t pop_count_table_8bit(uint64_t n) {
      int res = 0;
      uint8_t *p = (uint8_t *)&n;
      for (int i = 0; i < 8; ++i) {
        res += pop_count_table_ptr[*(p + i)];
      }
      return res;
    }
};

class HammingDistanceTable16Bit : public Algorithm {
  public:
    std::string getName() {
      return "HammingDistanceTable16Bit";
    }
    void init() {
      pop_count_table_ptr = NULL;
      pop_count_table_16bit_init(&pop_count_table_ptr);
    }
    
    uint64_t cal(const uint64_t* p, const uint64_t* q, const uint64_t size) {
      uint64_t res = 0;
      for (uint64_t i = 0; i < size; ++i)
      {
        uint64_t r = (*(p + i)) ^ (*(q + i));
        res += pop_count_table_16bit(r);
      }
      return res;
    }
  private:
    uint8_t *pop_count_table_ptr; 
    
    void pop_count_table_16bit_init(uint8_t **pop_count_table_ptr) {
      *pop_count_table_ptr = new uint8_t[65536];
      for (int i = 0; i < 65536; ++i) {
        (*pop_count_table_ptr)[i] = __builtin_popcount(i);
      }  
    }

    uint64_t pop_count_table_16bit(uint64_t n) {
      int res = 0;
      uint16_t *p = (uint16_t *)&n;
      for (int i = 0; i < 4; ++i) {
        res += pop_count_table_ptr[*(p + i)];
      }
      return res;
    }
};



class HammingDistanceDivideConquer : public Algorithm {
  public:
    std::string getName() {
      return "HammingDistanceDivideConquer";
    }
    
    uint64_t cal(const uint64_t* p, const uint64_t* q, const uint64_t size) {
      uint64_t res = 0;
      for (uint64_t i = 0; i < size; ++i)
      {
        uint64_t r = (*(p + i)) ^ (*(q + i));
        res += pop_count_divide_conquer(r);
      }
      return res;
    }

    uint64_t pop_count_divide_conquer(uint64_t n) {
      n = (n & 0x5555555555555555) + ((n >> 1 ) & 0x5555555555555555); 
      n = (n & 0x3333333333333333) + ((n >> 2 ) & 0x3333333333333333);
      n = (n & 0x0F0F0F0F0F0F0F0F) + ((n >> 4 ) & 0x0F0F0F0F0F0F0F0F);
      n = (n & 0x00FF00FF00FF00FF) + ((n >> 8 ) & 0x00FF00FF00FF00FF);
      n = (n & 0x0000FFFF0000FFFF) + ((n >> 16) & 0x0000FFFF0000FFFF);
      n = (n & 0x00000000FFFFFFFF) + ((n >> 32) & 0x00000000FFFFFFFF);
      return n;
    }
  private:

};

class HammingDistanceDivideConquerOpt : public Algorithm {
  public:
    std::string getName() {
      return "HammingDistanceDivideConquerOpt";
    }
    
    uint64_t cal(const uint64_t* p, const uint64_t* q, const uint64_t size) {
      uint64_t res = 0;
      for (uint64_t i = 0; i < size; ++i)
      {
        uint64_t r = (*(p + i)) ^ (*(q + i));
        res += pop_count_divide_conquer_opt(r);
      }
      return res;
    }

    uint64_t pop_count_divide_conquer_opt(uint64_t n) {
      n = n - ((n >> 1)  & 0x5555555555555555); 
      n = (n & 0x3333333333333333) + ((n >> 2 ) & 0x3333333333333333);
      n = (n + (n >> 4 )) & 0x0F0F0F0F0F0F0F0F;
      n = n + (n >> 8 );
      n = n + (n >> 16);
      n = n + (n >> 32);
      return (uint64_t)(n & 0x7F);
    }
  private:

};

int main() {
  AlgorithmBench algorithm_bench;
  algorithm_bench.setSize(10000000);
  algorithm_bench.push_back(new HammingDistanceBase);
  algorithm_bench.push_back(new HammingDistanceBaseAsm);
  algorithm_bench.push_back(new HammingDistanceBuildin);
  algorithm_bench.push_back(new HammingDistanceTable8Bit);
  algorithm_bench.push_back(new HammingDistanceTable16Bit);
  algorithm_bench.push_back(new HammingDistanceDivideConquer);
  algorithm_bench.push_back(new HammingDistanceDivideConquerOpt);
  algorithm_bench.init();
  algorithm_bench.start();
  return 0;
}
