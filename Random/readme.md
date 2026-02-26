랜덤 함수의 유효율 측정
=============


### 각 랜덤 함수의 유효 범위를 측정합니다.

#### 일자
2026 02 26

#### 환경
Visual Studio2022, C++14, Release   

CPU: ADM Ryzen 5 7500F 6-Core Processor   

GPU: NVIDIA GeForce RTX 4060 Ti   

결과
| rand   | xor shift random | mt19937      |
| ------ | ---------------- | ------------ |
| 0.001% | over 0.0001%     | over 0.0001% |
   
### LOG
2026 02 26 - 생성