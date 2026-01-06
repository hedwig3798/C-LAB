분기예측 실패 지연 분석
============


### 분기 예측에 따른 지연시간 분석

#### 일자
2025 01 06

#### 환경
Visual Studio2022, C++14, Release   

CPU: ADM Ryzen 5 7500F 6-Core Processor   

GPU: NVIDIA GeForce RTX 4060 Ti   
   

방법   
1억개의 vector를 순회하면서 if 문이 있을떄, 없을때, if문이 있지만 정렬되어 있을 때 3가지를 비교 

   
결과
| 환경                | 시간 (s)   |
| ----------------- | -------- |
| Branch            | 0.236033 |
| Branchless        | 0.023897 |
| Branch but Sorted | 0.022808 |
   
### LOG
2026 01 06 - 생성
