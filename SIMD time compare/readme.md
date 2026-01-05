SIMD 연산 효율 측정
=============


### SIMD 를 사용했을때와 아닐때의 연산 결과를 비교합니다.

#### 일자
2025 03 19

#### 환경
Visual Studio2022, C++14, Release
CPU: ADM Ryzen 5 7500F 6-Core Processor
GPU: NVIDIA GeForce RTX 4060 Ti

결과 ( 단위 s )
| 환경       | 시간 (s)   |
| -------- | -------- |
| Set Data | 0.359071 |
| Array    | 1.50553  |
| SSE Slow | 1.5373   |
| SSE      | 1.55026  |

### LOG
2025 03 19 - 생성