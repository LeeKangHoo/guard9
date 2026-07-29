# guard9

<p>
  <img width="622" height="354" alt="guard9 실행 화면" src="https://github.com/user-attachments/assets/b7a34a3a-a51e-4553-b2a5-e004ccceed63">
</p>

Netfilter는 리눅스 커널 내부에서 네트워크 패킷을 검사하고, 수정하거나 차단할 수 있도록 제공되는 프레임워크입니다.

Netfilter를 활용해 개발한 리눅스용 네트워크 보안 프로젝트입니다.

## 주요 기능

* 도메인 접근 차단 기능
* IP 기반 차단 기능
* Port 기반 차단 기능

## 사용 방법

<p>
  <img width="622" height="354" alt="guard9 실행 화면" src="https://github.com/user-attachments/assets/b7a34a3a-a51e-4553-b2a5-e004ccceed63">
</p>
메뉴 선택 화면

<p> <img width="691" height="437" alt="image" src="https://github.com/user-attachments/assets/e95d4f0d-efac-42cb-b6bd-347857a50e2b" /></p>
1번 : 차단할 도메인 추가 / 삭제
<p></p><img width="691" height="430" alt="image" src="https://github.com/user-attachments/assets/36a3b753-3474-468b-9c33-c8d27d252b41" /></p>
2번 : 차단할 IP 추가 / 삭제
<p><img width="691" height="431" alt="image" src="https://github.com/user-attachments/assets/fec5bcff-2bb3-4fd2-bd14-b742eb27f58b" /></p>
3번 : 차단할 Port 추가 / 삭제
<p><img width="1603" height="998" alt="image" src="https://github.com/user-attachments/assets/2ead9e2a-2256-4200-b7fe-d50042c0bfa4" />
</p>
<p><img width="686" height="433" alt="image" src="https://github.com/user-attachments/assets/aad818bc-f258-4a55-ad3f-e4b5bb474801" />
</p>
실제 차단이 되는 모습

## 설치 및 실행 환경 설정
테스트한 환경은 다음과 같습니다.
- Fedora Linux 44 (Workstation Edition)
- 커널 버전 : Linux 7.1.4-200.fc44.x86_64
- g++ (GCC) 16.1.1 20260515 (Red Hat 16.1.1-2) (c++ 17이상)
- CMake 3.20이상 (테스트 환경은 4.3.0)
- nftables
- libnetfilter_queue 라이브러리

**1. 필수 패키지 설치 (Fedora 계열)**
```bash
sudo dnf install \
    gcc-c++ \
    cmake \
    pkgconf-pkg-config \
    libnetfilter_queue-devel \
    nftables
```
**2. 프로젝트 빌드**

항상 다운받은경로/guard9/ 경로에서 실행해주세요!
debug 빌드
```bash
./script/build.sh
```
release 빌드
```bash
./script/build.sh release
```
빌드된 실행 파일은 다음 경로에 생성됩니다.
```bash
build-local/guard9
```
만약 실행 권한이 없다면
```bash
chmod +x scripts/*.sh
```

**3. nftable 설정**

반드시 root권한으로 실행해주세요!

nftable 설정
```bash
sudo ./scripts/setup-nft.sh
```
설정 초기화
```bash
sudo ./scripts/clean-nft.sh
```

**4. 프로그램 실행**

반드시 root권한으로 실행해주세요!
```bash
sudo ./buold-local/guard9
```

## 사용한 라이브러리 및 원리 설명
Libnetfilter_queue 라이브러리는 Linux 커널의 Netfilter가 NFQUEUE로 패킷을 전달해 userspace에서 패킷을 처리할 수 있도록 해줍니다.
단순히 패킷을 복사해 userspace에서 읽을 수 있게하는 pcap라이브러리와는 다르게 verdict함수를 통해 DROP할지, ACCEPT할지 등 패킷을 실제로 holding해 처리할 수 있도록 해줍니다.

nftables는 패킷을 NFQEUE에 전달하기 위한 Linux방화벽 설정 도구입니다. 설정한 정책에 따라 패킷을 NFQUEUE에 전달합니다. (해당 프로그램에서는 NFQUEUE 0번을 사용하니 기존에 사용하고 있었다면 주의해주세요)

**특이사항**

Q : 왜 detect가 여러번 되나요?<br>
A : 브라우저는 연결이 실패할시 재시도를 하게 됩니다. 재시도 또한 input또는 output패킷으로 똑같이 간주하기 때문에 실제로 여러번 차단합니다. 

Q : 차단은 어떻게 하나요?<br>
A : 첫 번째로 큐로 들어온 패킷을 DROP해버립니다. 두 번째로 port와 sni는 rst 패킷을 만들어 보냅니다.

Q : 도메인 차단 기능이 될때가 있고 안될때가 있어요<br>
A : 도메인 차단 기능은 TLS handshake때 클라이언트가 보내는 Client Hello에서 sni에서 평문으로 보낸다는 것을 이용해 차단을 합니다.(아직 키 교환을 하지 않았기 때문) 따라서 이미 연결이 되어버린 상황에서는 차단이 되지 않습니다. ( 보통 이미 연결을 해두고 guard9을 켰을때 문제가 생김)


