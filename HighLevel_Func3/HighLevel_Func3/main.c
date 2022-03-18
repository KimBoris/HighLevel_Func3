#include <stdio.h>




/*
	<정적 라이브러리의 구현>

	- 정적 라이브러리 

	Hello.c  ---->    Hello.obj   ---->    Hello.exe
	(고급어)   |			(기계어)		|		(기계어)
			Compile			|	  Link
							|
							|
					기계어 번역이 끝난것
					But, 실행가능한 기계어 상태는 아님

	1. Hello.c를 만들면 = 이것은 고급언어
	2. 이것을 컴파일 하게 되면 -> Hello.obj가 된다.
	3. Hello.obj -> 링크 과정을 거치면 Hello.exe가 된다.(실행파일)
	4. Hello.obj, Hello.exe 둘다 기계어가 맞다.
	5. Hello.obj = 실행 가능한 형태의 기계어는 아니다.



	<과거 수업에서 분할 컴파일을 하였다>

	main()				TestFunc()
	   |		            |
	Hello.c				Test.c

	- Hello.c와 Test.c이 두 파일을 합쳐 Hello.exe가 되었다.
	
	----------------------------------
	|     Hello.c       |  Test.c    |
	----------------------------------
	- 이것이 Hello.exe 파일형태이다.

	[실행순서도]
	- Hello.c에서 쭉 코드가 실행되다 Test.c가 호출되면 Test.c부분을 쭉 실행시키고 다시 
	  Test.c가 호출되었던 Hello.c영역에서 부터 코드가 쭉 구현된다.

	*그런데 TestFunc()함수가 너무 자주 쓰인다면?
	
	(ex)
	그리고 프로그램 World.c가 하나 더 있다면? 
	World.c -> World.obj -> World.exe
	
	- TestFunc()가 있는 Test.obj파일을 Test.lib파일로 변경해서 main(), world.c에서 원할때 call을 할 수 있도록 해준다.



	<library 라이브러리>
	- 공구상자 ( 여러가지 도구가 들어있는 )
	1. 'a'를 수행할때 3가지가 필요하다
	2. 'b'를 수행하는데 10가지가 피룡하다
	3. 그럼 1,2는 서로 겹치는 도구도 있고 그렇지 않은 도구도 있을것이다.

	* 함수 > 함수를 도구로 봤을때 라이브러리란?
	 - 도구를 모아두는 상자
	 - 그래서 일을 할 때 우리는 단지 함수를 call해서 하면 된다.
	 - .lib = 사실 .obj와 유사하다.

*/


/*
	<정적 라이브러리 만들기>
	
	[TestApp.c]
	void PrintValue(int);//함수 원형 선언
	#pragma comment(lib,"..\\Debug\\LibTest.lib") - lib파일을 사용한다.

	int main(void)
	{
		PrintValue(10);
		PrintString("Static Library Test");
		return 0;
	}

	-> 값을 '10' , "Static library Test" 가 나온다.



	[LibTest.c] (lib파일이 생성)
	자주 쓸것같은 함수를 모아놓은 스크립트

	#include <stdio.h>
	//이코드에는 main이 없다.

	void PrintValue(int nparam)
	{
		printf("%d\n", nParam);
	}
	void PrinfString(const char *pszParam)
	{
		printf("%s\n", pszParam);
	}

	=====================================
	1. 여기에 LibTest.h(헤더파일)을 만들어서
	2. void PrintValue(int);
	   #pragma comment (lib, "..\\Debug\\LibTest.lib");를 작성해준다.
	3. #pragma once를 작성해준다.
	4. 그럼 이것을 TestApp가 가져다 쓸데에는 
	#include "..\\LibTest\\LibTest.h" 이렇게 작성해주면 된다. 
	(시작 프로젝트로 설정하는것 잊지말고)
	==========================================================


	<정리>

	- 내가 자주 사용할 것 같은 함수를 'lib'파일로 만들어 놓고 
	  필요할 때에 (프로젝트가 추가되어도 상관없이 헤더 선언만 해주면됨) 사용함

	- #include "..\\LibTest\\LibTest.h"를 선언하여 자유롭게 사용 
		사용전 '시작 프로젝트로 설정' 체크하기

	- #pragma once -> 헤더 파일이 중복으로 Include되었을 때 
						 한 번만 유효한 것으로 처리하도록 컴파일러에게 알리는 역할






	<최종>
	- lib = obj 파일이라고 생각하면된다.

	- lib - Link할 때 필요함 - 모듈화 프로그램 ( 나눠서 프로그래밍하는것)

	- 함수를 쪼개놓으면 나중에 재활용 하기 쉽다




	<SDK란?>
	software developer kit
	소프트웨어 개발 키트 
	 - lib의 모음 < call + Link 해주면 끝이다.


	 <동적 lib>
		
    - 정적과 정 반대 > DLL파일 (Dynamic Linking Library)
	1. ~.c - 소스코드 제작시
	2. ~.h - 컴파일 할때
	3. ~.lib - 링크할때
	4. ~.DLL - 런타임 할 때 필요함




	<정적 라이브러리 질문>
	- 땡겨온 Library들 끼리 충돌이 날 수 있나?
		= 물론이다. 이름이 같을때



		(ex)
			exe파일에서 PrintValue(int)를 당겨온다면

[LibTest.lib]			[World.lib]
PrintValue(int)			PrintValue(int)

> 필요한 라이브러리를 다 땡겨오는데 이름이 같거나 겹치면 '링크 오류'가 난다.



	<실무에서 충돌나는 가장 큰 이유>

	- 내부적으로 어떤 라이브러리에서 '소켓'같은 것을 통신하겠다고 
	Socket초기화를 하고 앉았는데 world.lib가 PrintValue(int)를 딱 불러보면
	Socket이 초기화가 됐던 말던 자기 입장만 생각하고, 
	만약 해제 해버린다면?!
	exe를 개발할 때 앞 뒤 순서가 맞지 않는 경우가 생긴다.


	*링크 오류(중복) = 이름을 바꿔주면 된다. - C언어에서 적용
	*namespace를 사용한다 - C++

	

	<정적, 동적 라이브러리중 선호>

	 요즘은 대부분 DLL이다.

	 -> 정적 라이브러리의 단점

	 1. LibTest.lib에에 있는 PrintValue()함수를 사용하면 
	 |-----------|
	 |LibTest.lib|
	 |PrintValue-|
	 |-----------|
	  이렇게 안에 담겨있는데 

	  2.a, b 실행파일을 만들때
		
	----------------------------------
	|     a.exe      |  PrintValue() |
	----------------------------------

	----------------------------------
	|     .exe      |  PrintValue() |
	----------------------------------
	이런식으로 찰싹 붙게된다.

	3. 그런데 lib파일의 PrintValue()에서 수정이 일어났다면 exe파일에 있는 것도
	다시 수정을 해줘야 한다는 문제점이 있다.

	만약, DLL이라면?
	------------------
	|     a.exe      |
	------------------

	-----------------
	|    b.exe      |
	-----------------

	-----------------
	|  PrintValue() |
	-----------------

	> 위와 같이 생겨 필요하면 Call하는 방식이기에 DLL을 더 선호한다.






	<DLL HOOK>
	(주소 치환)
	
	- 특정 함수가 호출될 때 그 호출을 가로채는 경우도 있다.
	- DLL형태의 악성코드로 사용되기도 한다. 





	-끝-















*/
