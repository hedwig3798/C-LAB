std::vector 의 삭제 연산을 보완한 2가지 방법의 벡터
=============


### 3 종류 벡터의 삭제 연산 속도를 비교합니다.

#### 일자
2026 09 15

#### 환경
Visual Studio2022, C++20, Release   

CPU: ADM Ryzen 5 7500F 6-Core Processor   

GPU: NVIDIA GeForce RTX 4060 Ti   

결과
| 단위 / ms         | 삭제      | 삭제 후 순회 |
| --------------- | ------- | ------- |
| std::vector     | 114.142 | 0.0203  |
| swap pop vector | 0.2216  | 0.0214  |
| slot vector     | 0.3362  | 0.3317  |
   
### LOG
2026 09 15 - 생성