# TIL: Spring Security 인증 핵심 구조

**날짜**: 2026-04-21
**주제**: Authentication, UserDetailsService, AuthenticationManager, PasswordEncoder 관계 이해

---

## 한 줄 요약

> `AuthenticationManager`는 인증의 **단일 진입점**이고, 실제 일은 `AuthenticationProvider`에 위임한다. `DaoAuthenticationProvider`는 `UserDetailsService`로 사용자 정보를 조회하고, `PasswordEncoder`로 비밀번호를 검증한다. `Authentication` 객체는 **인증 전/후** 두 얼굴을 가진다.

---

## 핵심 개념 정리

### 1. Authentication 인터페이스의 두 모습

같은 타입의 객체인데, 상황에 따라 의미가 정반대다.

| 구분 | 인증 전 (Before) | 인증 후 (After) |
|---|---|---|
| 생성 주체 | `UsernamePasswordAuthenticationFilter` | `AuthenticationProvider` |
| `principal` | username 문자열 | `UserDetails` 객체 |
| `credentials` | raw password | `null`로 지워짐  |
| `authorities` | 빈 컬렉션 | 권한 목록 채워짐 |
| `isAuthenticated()` | `false` | `true` |

**인증 후 credentials가 지워지는 이유**: 메모리 덤프/로그 유출로부터 평문 비밀번호 보호. `ProviderManager`가 `CredentialsContainer` 인터페이스 구현체의 credentials를 자동으로 비운다.

**공식 문서 원문**: Authentication 인터페이스는 "AuthenticationManager에게 자격증명을 제공하는 입력" 또는 "현재 인증된 사용자의 표현"이라는 두 목적을 가진다.

---

### 2. UserDetailsService 

**문제**: 애플리케이션마다 사용자 저장 방식이 다르다 (PostgreSQL, MongoDB, LDAP, 외부 REST API…).

**해결**: Spring Security는 저장 방식을 강제하지 않고 **Strategy 패턴**으로 계약만 정의한다.

```java
public interface UserDetailsService {
    UserDetails loadUserByUsername(String username)
        throws UsernameNotFoundException;
}
```

메서드 딱 하나. "username만 줄 테니 UserDetails만 돌려줘. 어떻게 가져올지는 네가 정해."

**호출 시점**: `DaoAuthenticationProvider.authenticate()` 내부의 `retrieveUser()` 단계에서 호출된다. **로그인 요청마다 1회**. 이후 세션 유지 요청에서는 `SecurityContextHolder`에서 꺼내 재사용하므로 DB를 다시 치지 않는다.

---

### 3. AuthenticationManager → AuthenticationProvider 위임

**왜 둘로 나눴나?** 한 앱에 Form 로그인 + JWT + LDAP이 공존할 수 있기 때문. 필터마다 if-else로 분기하지 않도록 **전략을 분리**한다.

- `AuthenticationManager`: 단일 진입점 (보통 `ProviderManager`가 구현)
- `AuthenticationProvider`: 특정 방식의 인증 전문가 (`DaoAuthenticationProvider`, `JwtAuthenticationProvider` 등)

ProviderManager 동작 과정은 다음과 같다.
1. 등록된 Provider들을 순회
2. `provider.supports(tokenClass)`가 true인 것만 실행
3. 성공한 Provider가 있으면 즉시 break
4. `AccountStatusException`(계정 잠김 등)은 즉시 실패, 다음 Provider 시도 안 함
5. 일반 `AuthenticationException`은 다음 Provider 시도
6. 아무도 처리 못 하면 `ProviderNotFoundException`

DaoAuthenticationProvider의 진행 과정은 다음과 같다.
1. 필터가 `UsernamePasswordAuthenticationToken`을 `ProviderManager`에 전달
2. `ProviderManager`가 `DaoAuthenticationProvider` 선택
3. `UserDetailsService.loadUserByUsername()` 호출 → `UserDetails` 획득
4. `PasswordEncoder.matches(rawPwd, storedHash)`로 검증
5. 성공 시 인증된 토큰 생성하여 반환

---

### 4. PasswordEncoder와 BCrypt

**BCrypt가 안전한 이유 두 가지**

**(1) Salt 내장**

```
$2a$10$ZLhnHxdpHETcxmtEStgpI./Ri1mksgJ9iDP36FmfMdYyVg9g0b2dq
│  │  │ └─────── salt (22자) ──────┘└─── hash (31자) ───┘
│  │  └── cost (work factor) = 10
│  └───── 버전: $2a, $2b, $2y
└──────── prefix
```

→ salt를 별도 컬럼으로 관리할 필요 없음. 해시 안에 다 들어있다.

**(2) Work Factor (오늘의 핵심)**

Work factor = 해싱 반복 횟수의 `log₂`. 실제 반복 횟수는 `2^work_factor`.

| log_rounds | 실제 라운드 | 상대 시간 |
|---|---|---|
| 10 (기본) | 1,024 | 1x |
| 12 | 4,096 | 4x |
| 14 | 16,384 | 16x |
| 16 | 65,536 | 64x |

→ **1 올릴 때마다 2배 느려짐.** 사용자는 1초 기다리지만, 공격자가 10억 해시 크래킹하려면 31년 이상이 걸릴것이다.

**공식 권장**: 시스템에서 검증에 약 1초 걸리도록 튜닝. OWASP 권장값은 10, 실무에서는 10~14.

---

### 5. DelegatingPasswordEncoder가 기본값인 이유

Spring Security 5부터 기본은 `BCryptPasswordEncoder`가 **아니라** `DelegatingPasswordEncoder`.

해시에 prefix를 붙여 알고리즘을 표시:
```
{bcrypt}$2a$10$GRLdNijSQMUvl/au9ofL...
{noop}password
{argon2}$argon2id$v=19$m=16384,t=2,p=1$...
```

→ 레거시 알고리즘에서 최신 알고리즘으로 **점진 마이그레이션** 가능. 프레임워크가 breaking change를 내지 않으면서도 best practice 변화에 대응하기 위한 설계이다.

---

## 코드 예제 

```java
@Configuration
@EnableWebSecurity
public class SecurityConfig {

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder(12);  // work factor 12
    }

    @Bean
    public UserDetailsService userDetailsService(PasswordEncoder encoder) {
        UserDetails alice = User.builder()
                .username("alice")
                .password(encoder.encode("s3cret!"))
                .roles("USER")
                .build();
        return new InMemoryUserDetailsManager(alice);
    }

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            .authorizeHttpRequests(auth -> auth
                .requestMatchers("/", "/public/**").permitAll()
                .anyRequest().authenticated()
            )
            .formLogin(Customizer.withDefaults())
            .logout(Customizer.withDefaults());
        return http.build();
    }
}
```

**확인**: `/secret` 접속 → `/login` 리다이렉트 → `alice` / `s3cret!` 입력 → 성공.

---

## 핵심 내용

**Q1. BCrypt Work Factor가 뭐고 왜 중요한가?**

BCrypt 내부 해싱 반복 횟수의 `log₂`. 1 올리면 2배 느려짐(지수적). 공격자의 브루트포스 비용을 증가시키는 레버이며, 하드웨어 발전에 맞춰 값을 올려 대응할 수 있다는 점이 "adaptive" 의미의 핵심. 공식 권장: 검증에 ~1초 걸리도록 튜닝. OWASP 권장 10, 실무 10~14. 31은 CVE-2022-22976 때문에 금지.

**Q2. UserDetailsService.loadUserByUsername() 호출 시점?**

`DaoAuthenticationProvider.authenticate()`가 `ProviderManager`로부터 위임받은 후, 내부 `retrieveUser()` 단계에서 호출. **로그인 요청마다 1회**. 세션 유지 요청에서는 `SecurityContextHolder` 재사용이므로 호출되지 않음.

---

## 레퍼런스

- [Servlet Authentication Architecture](https://docs.spring.io/spring-security/reference/servlet/authentication/architecture.html)
- [DaoAuthenticationProvider](https://docs.spring.io/spring-security/reference/servlet/authentication/passwords/dao-authentication-provider.html)
- [Password Storage](https://docs.spring.io/spring-security/reference/features/authentication/password-storage.html)
- [In-Memory Authentication](https://docs.spring.io/spring-security/reference/servlet/authentication/passwords/in-memory.html)
- [ProviderManager JavaDoc](https://docs.spring.io/spring-security/reference/api/java/org/springframework/security/authentication/ProviderManager.html)
- [CVE-2022-22976](https://spring.io/security/cve-2022-22976/)
- [Baeldung - BCrypt Password Encoding](https://www.baeldung.com/spring-security-registration-password-encoding-bcrypt)