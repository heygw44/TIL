# TIL: JWT 인증 정리

---

## 1. JWT의 본질

### 1.1 표준과 구조

세 파트가 점(`.`)으로 구분된다.
 
```
eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiIxMjMifQ.SflKxwRJSMeKKF...
  └───── Header ─────┘ └─── Payload ───┘ └── Signature ──┘
        (Base64Url)       (Base64Url)        (Base64Url)
```
 
**Header** — 토큰 타입과 서명 알고리즘 선언
```json
{ "alg": "HS256", "typ": "JWT" }
```
 
**Payload** — Claim(주장)들의 집합. 대표적인 표준 Claim은 다음과 같다.
 
| Claim | 의미 |
|---|---|
| `iss` | Issuer (발급자) |
| `sub` | Subject (주로 userId) |
| `aud` | Audience (수신 대상) |
| `exp` | Expiration (만료 Unix timestamp) |
| `iat` | Issued At (발급 시각) |
| `jti` | JWT ID (고유 식별자, 블랙리스트용) |
 
**Signature** — `HMAC-SHA256(header + "." + payload, secret)` 같은 형태로 계산된 값.

### 1.2 주의 할 점
Payload는 디코딩만 하면 평문이 그대로 노출된다.

RFC 7519는 두 종류를 정의하는데
- **JWS** (JSON Web Signature) — 서명된 JWT. Payload는 누구나 읽을 수 있음
- **JWE** (JSON Web Encryption) — 암호화된 JWT. Payload가 실제로 가려짐
**우리가 일반적으로 말하는 "JWT"는 사실상 JWS**. 즉 암호화가 아니라 서명이다.
 
### 1.3 그러면 서명은 뭘 보장하나?
 
- **무결성(Integrity)** — Payload가 중간에 변조되지 않았음
- **진정성(Authenticity)** — 우리 서버가 발급한 게 맞음
- **기밀성(Confidentiality)** — 보장 안 됨

### 1.4 여기서 유도되는 실무 원칙
 
1. **Payload에 민감정보 절대 금지** — 비밀번호, 주민번호, 카드번호 등. `userId`, `role` 정도만.
2. **HTTPS는 필수** — 어차피 Payload는 평문이나 마찬가지라서 전송 구간 암호화는 TLS에 맡김.
3. **"JWT 디코딩"을 "보안 해제"로 착각하지 말 것** — 토큰 디코딩은 누구나 가능, 서명 검증이 실제 보안을 담당한다.

### 1.5 주의해야할 취약점 
 
과거 많은 라이브러리가 `alg: none`으로 설정된 토큰을 그대로 받아들여 인증을 우회당함. 검증 시 **허용 알고리즘을 화이트리스트로 고정**해야 한다.
 
```java
// 헤더의 alg를 그대로 신뢰 -> 위험
Jwts.parser().setSigningKey(key).parseClaimsJws(token);
 
// HS256만 허용 -> 신뢰적
Jwts.parser()
    .verifyWith(key)
    .requireAlgorithm("HS256")
    .build()
    .parseClaimsJws(token);
```
---

## 2. HS 256 vs RS256

### 2.1 동작 차이
 
```
HS256
  [서명 생성]  secret + (header.payload) → HMAC-SHA256 → signature
  [서명 검증]  secret + (header.payload) → HMAC-SHA256 → 같은가?
  → 발급자와 검증자가 같은 secret을 공유해야 함
 
RS256
  [서명 생성]  private key로 서명
  [서명 검증]  public key로 검증 (누구나 가능)
  → 검증자는 public key만 있으면 됨
```
 
### 2.2 선택 기준 (실무 판단)
 
| 상황 | 권장 |
|---|---|
| 단일 모놀리식 서버가 발급·검증 | **HS256** |
| MSA — 발급은 Auth 서버, 검증은 여러 서비스 | **RS256** |
| 외부 시스템이 내 토큰을 검증해야 함 | **RS256** |
 
성능 차이는 일반 웹 앱 수준에서 **무시 가능**. HS256은 수십 마이크로초, RS256(2048bit)은 단일 밀리초 수준.

---

## 3. Spring Security 필터 체인에서의 위치

### 3.1 Spring Security는 필터 체인이다
 
요청이 컨트롤러에 도달하기 전에 여러 필터가 순서대로 실행됨. Spring Security가 구성하는 기본 체인은 다음과 같다.
 
```
DisableEncodeUrlFilter
→ WebAsyncManagerIntegrationFilter
→ SecurityContextHolderFilter        (SecurityContext 로드)
→ HeaderWriterFilter
→ LogoutFilter
→ UsernamePasswordAuthenticationFilter   ← 기준점
→ RequestCacheAwareFilter
→ ...
→ ExceptionTranslationFilter         (필터 예외 → Entry Point)
→ AuthorizationFilter                (인가 검사)
```
 
### 3.2 커스텀 JWT 필터는 어디에 위치하는가
 
**`UsernamePasswordAuthenticationFilter` 앞**이 컨벤션.
 
```java
.addFilterBefore(jwtAuthenticationFilter,
    UsernamePasswordAuthenticationFilter.class);
```
 
- 폼 로그인 필터보다 먼저 JWT 인증을 시도
- 뒤쪽 `AuthorizationFilter`는 `SecurityContext`의 `Authentication`을 보고 판단하므로, 그 전에 세팅되어야 함
### 3.3 `OncePerRequestFilter` 를 상속하는 이유
 
요청 하나당 **정확히 한 번만** 실행되는 필터. 일반 `Filter`는 forward/include 시 여러 번 호출될 수 있다. JWT 검증 같은 인증 로직은 중복 실행되면 다음과 같은 문제가 발생할 수 있다.
- 성능 낭비
- 예상치 못한 상태 변이
그래서 보안 용도엔 `OncePerRequestFilter`가 표준.
 
### 3.4 필터의 유일한 책임
 
```java
@Component
@RequiredArgsConstructor
public class JwtAuthenticationFilter extends OncePerRequestFilter {
 
    private final JwtService jwtService;
    private final UserDetailsService userDetailsService;
 
    @Override
    protected void doFilterInternal(
            HttpServletRequest request,
            HttpServletResponse response,
            FilterChain filterChain) throws ServletException, IOException {
 
        // 1. Authorization 헤더에서 토큰 추출
        String authHeader = request.getHeader("Authorization");
        if (authHeader == null || !authHeader.startsWith("Bearer ")) {
            filterChain.doFilter(request, response);  // 없으면 통과 (permitAll 대응)
            return;
        }
        String token = authHeader.substring(7);
 
        // 2. 토큰 검증 + username 추출
        String username = jwtService.extractUsername(token);
        if (username != null
            && SecurityContextHolder.getContext().getAuthentication() == null) {
 
            UserDetails userDetails = userDetailsService.loadUserByUsername(username);
            if (jwtService.isTokenValid(token, userDetails)) {
                // 3. SecurityContext에 Authentication 심기 ← 이게 핵심 책임
                UsernamePasswordAuthenticationToken auth =
                    new UsernamePasswordAuthenticationToken(
                        userDetails, null, userDetails.getAuthorities());
                SecurityContextHolder.getContext().setAuthentication(auth);
            }
        }
 
        // 4. 다음 필터로
        filterChain.doFilter(request, response);
    }
}
```
 
이 필터가 하는 일은 딱 하나 이다. 유효한 토큰이면 `SecurityContextHolder`에 `Authentication` 심는다. 이후 `AuthorizationFilter`가 그 `Authentication`을 보고 인가 여부를 판단한다.
 
---

## 4. Access Token + Refresh Token 설계
 
### 4.1 왜 토큰을 둘로 나누어야할까?
 
단일 토큰의 딜레마는 다음과 같다.
- **수명 길게** → 탈취 피해 시간이 길어짐
- **수명 짧게** → 사용자가 10분마다 재로그인

하지만 이를 분리해서 설계한다면?
- **Access Token** — 짧음(15~60분), API 호출마다 전송, stateless 검증
- **Refresh Token** — 길음(7~30일), AT 재발급 때만 사용, 서버에 저장
### 4.2 표준 흐름
 
```
[로그인 성공]
Server → Client: { accessToken(15분), refreshToken(14일) }
 
[API 호출]
Client → Server: Authorization: Bearer <accessToken>
 
[accessToken 만료 → 401]
Client → Server: POST /auth/refresh { refreshToken }
Server → Client: { accessToken(새것), refreshToken(새것) }   ← 회전
```
 
### 4.3 Refresh Token Rotation (RTR)
 
**매 재발급 시 RT도 새로 발급 + 이전 RT는 즉시 무효화.**
 
```
정상:  RT1 → RT2 → RT3 → RT4 ...  (이전 것은 모두 무효)
 
공격 감지:
  RT1 → RT2 (정상 사용자가 쓴 후)
        ↓
  공격자가 훔친 RT1로 재시도
        ↓
  서버: "이미 사용된 토큰이 또 왔네" → 해당 사용자의 모든 RT 무효화 → 강제 재로그인
```
 
이게 **Reuse Detection**. OAuth 2.1 권장 사항이다.
 
### 4.4 왜 RT만 DB에 저장할까?
 
**Access Token은 stateless로 OK, Refresh Token은 stateful이 되어야 한다.** 그 이유는 다음과 같다.
 
1. **즉시 무효화 필요** — AT는 15분 후 자연 만료되지만, RT가 탈취되면 14일간 치명적
2. **재사용 감지를 위해 상태 필요** — "이미 썼는지"를 알아야 함
3. **로그아웃/강제 만료** — DB에서 RT를 지우면 즉시 세션 종료
4. **디바이스 관리** — "다른 기기에서 로그아웃" 기능 구현 가능
저장 방식:
- **Redis 권장** (TTL 네이티브 지원, 자동 청소, sub-ms 조회)
- 원본 X, 해시(SHA-256)로 저장 (DB 유출 대비)
- 키 설계: `refresh_token:{userId}:{deviceId}` 같은 식

### 4.5 RT를 저장하면 결국 세션과 같은 방식일까?
 
**RT 저장 = 사실상 유효기간이 긴 세션**. 이름만 JWT일 뿐이다.
 
이 점을 이해하면 설계 논의가 명확해진다.
- **완전한 stateless는 비현실적** — 로그아웃도 못 하고 탈취 대응도 못 함
- **실무 JWT = "stateless AT + stateful RT" 하이브리드**
- 이 구조가 Auth0, OAuth 2.1의 표준이다.
---

## 5. 실무 현실 — 세션 vs JWT vs 하이브리드
 
### 5.1 기사 하나로 정리되지 않는 현실
 
찾아보면서 가장 인상적이었던 건: **"JWT가 모던, 세션이 구식"은 오해**라는 사실.
 
> *"안전하다는 이유만으로 세션 방식을 선택했다면 높은 확률로 틀린 거라고 할 수 있다... 하지만 JWT 토큰만으로 사용자 식별을 한다면 토큰 탈취 사고가 났을 때 할 수 있는 방법은 백엔드에서 signature를 바꾸는 것뿐이다."*
 
둘 다 트레이드오프가 있고, **맥락에 따라** 선택하는 것.
 
### 5.2 국내 대기업 실제 방식 (공개 자료 기준)
 
- **카카오** — 이중 구조
  - `accounts.kakao.com` 자체 로그인 = **세션 기반** (24시간, 로그인 유지 시 1달)
  - 외부 서비스용 카카오 로그인 = **OAuth 2.0 + JWT ID Token** (RS256 + JWKS)
- **라인** — 공식적으로 OAuth 2.0 + OIDC:
  - `access_token` (불투명 토큰)
  - `id_token` (JWT, 사용자 식별용)
  - `refresh_token` (서버 관리)
- **토스** — 금융 서비스답게 **mTLS** + 자체 인증 프로토콜 (JWT/세션 논의 초월)
 
**공통 패턴**: 순수 stateless JWT만 쓰는 곳은 거의 없음. **OAuth/OIDC 프레임워크 + 상황별 하이브리드**.
 
### 5.3 실무 4대 패턴
 
**패턴 A: 순수 세션** — 단일 도메인 웹, 어드민, 금융
 
**패턴 B: Spring Session + Redis** — 세션의 scale-out 대응
 
**패턴 C: JWT + DB 저장 RT** — 모바일/SPA의 표준
 
**패턴 D: BFF (Backend For Frontend)** — 최근 대기업 선호
```
[브라우저] ──세션 쿠키──→ [BFF] ──Bearer JWT──→ [MSA 서비스들]
                           ↕
                        [Redis: JWT 저장]
```
- 브라우저는 세션 장점 (XSS 방어, 즉시 로그아웃)
- MSA 내부는 JWT 장점 (stateless, 확장성)

현재 진행하는 도메인 및 프로젝트에서 어떤 패턴이 적합한지를 개발자 또는 팀에서 선택할 수 있어야한다. AI로 대부분의 코딩이 가능한 시점에 개발자가 갖춰야할 역량은 바로 이런것.

## 6. 자가 검증 Q&A
 
학습 후 스스로 답할 수 있는지 확인한 질문들이다.
 
### Q1. JWT와 세션의 근본적 차이는?
 
**저장 위치**. 세션은 인증 정보를 서버에 저장(stateful), JWT는 클라이언트에 저장(stateless). 이 차이에서 확장성·무효화·트래픽 비용 등 모든 트레이드오프가 파생됨.
 
### Q2. JWT Payload에 민감정보를 넣으면 안 되는 이유는?
 
JWT는 서명된 것이지 암호화된 게 아니라서, Payload를 Base64Url 디코딩만 하면 평문이 드러남. 누구나 볼 수 있음. 민감정보는 서버 DB에 두고 JWT에는 식별자만.
 
### Q3. Refresh Token을 DB에 저장해야 하는 이유는?
 
1. 즉시 무효화 필요 (14일짜리 토큰 탈취 시 치명적)
2. 재사용 감지를 위한 상태 필요
3. 로그아웃/강제 만료
4. 디바이스 관리
Access Token은 짧아서 stateless OK, Refresh Token은 stateful이 되어야 함.
 
### Q4. Access Token 탈취 시 대응은?
 
1. **짧은 수명** (15분) — 피해 창 최소화
2. **HTTPS 강제** — 전송 중 탈취 방지
3. **안전한 저장소** — localStorage 금지, HttpOnly 쿠키/Keychain 등을 사용하자.
4. **Refresh Token 회전** — AT 재발급 시 이상 감지
5. **jti + 블랙리스트** (고위험 환경)
### Q5. JWT에서 로그아웃이 왜 어려운가?
 
stateless라서. 세션은 서버에서 삭제하면 끝이지만, JWT는 토큰 자체가 유효성을 갖고 있어서 만료 전엔 계속 유효하다. 이를 해결하기 위해서는 다음과 같은 방법을 사용하자.
- 블랙리스트 (Redis jti) — stateful로 전환
- Token versioning — user 레코드에 version 필드
- 짧은 AT + RT 즉시 무효화 — 현실적 절충
### Q6. 401과 403의 차이?
 
- 401: 신원 증명 실패 (너 누구?)
- 403: 신원은 확인되었지만 권한 부족 (여긴 못 들어와)
### Q7. `@RestControllerAdvice`가 JWT 필터 예외를 못 잡는 이유?
 
필터는 DispatcherServlet **앞단**에서 실행. Advice는 Dispatcher **이후** 컨트롤러 예외만 처리. 필터 예외는 `ExceptionTranslationFilter` → `AuthenticationEntryPoint` / `AccessDeniedHandler` 경로로 별도 처리.
 
### Q8. `OncePerRequestFilter`를 왜 상속하나?
 
요청당 정확히 한 번만 실행 보장. 일반 Filter는 forward/include 시 중복 실행될 수 있어, 인증 로직 중복은 성능 낭비 + 예상치 못한 동작 유발.
 
### Q9. HS256과 RS256 중 언제 뭘 쓰나?
 
- 발급자 = 검증자 (단일 시스템) → HS256
- 발급자 ≠ 검증자 (MSA, 외부 시스템) → RS256
- 외부가 검증해야 함 (공개키 방식) → RS256