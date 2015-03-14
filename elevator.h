#include "inside.h"

#ifndef LINUX
#include <Windows.h>
#endif

// #define DEBUG_ELEVATOR

static int power_on = 0;	// 엘리베이터 전원 on/off 체크
static int cur_floor;		// 현재 층

// 엘리베이터 초기화
int elevator_power_on(void) {
	int i;
	int ok = 1;

	// 내부 버튼들 초기화
	if (power_on != 1) {
		for (i = 0; i < TOTAL_FLOORS; i++)
			inside.floor_button[i] = 0;

		inside.open_button = 0;
		inside.close_button = 0;
		// FIXME : 추후 파일 입출력을 통해 초기 값 저장하는 방법 구현
		cur_floor = 1;

		// 엘리베이터 전원 on
		power_on = 1;
#ifdef DEBUG_ELEVATOR
		printf("elevator_power_on OK!\n");
#endif
		return ok;
	}

	printf("elevator_power_on failed!\n");
	ok = 0;
	return ok;
}

void elevator_print_manual(void) {
	printf("\n\
				메뉴얼\n\n\
층-1	2	3	4	5	6	7	8	9	10\n\
bt:1	2	3	4	5	6	7	8	9	0\n\
▲:q	w	e	r	t	y	u	i	o	p\n\
▼:a	s	d	f	g	h	j	k	l	;\n\
\n\
층-11	12	13	14	15	16	17	18	19	20\n\
bt:!	@	#	$	%%	^	&	*	(	)\n\
▲:Q	W	E	R	T	Y	U	I	O	P\n\
▼:A	S	D	F	G	H	J	K	L	:\n\
\n\
◀▶:	z 또는 Z\n\
▶◀:	x 또는 X\n\
\n");
}

// 엘리베이터 활성된 버튼들 출력
void elevator_print_active_buttons(void) {
	int i;

	printf("Active buttons : ");

	for (i = 0; i < TOTAL_FLOORS; i++) {
		if (inside.floor_button[i])
			printf("%d ", i + 1);
	}

	if (inside.open_button)
		printf("◀▶ ");
	if (inside.close_button) {
		printf("▶◀ ");
	}

	printf("\n");
}

// 엘리베이터 led 출력 함수
void elevator_print_led(void) {
	printf("%d\n", cur_floor);
}

void elevator_print_to_screen(void) {
#ifndef DEBUG_ELEVATOR
#ifdef LINUX
		system("clear");
#else
		system("cls");
#endif
#endif /* DEBUG_ELEVATOR */
	elevator_print_active_buttons();
	printf("Current floor : ");
	elevator_print_led();
}

// 엘리베이터 문 열기 함수
void elevator_door_open(void) {
	clock_t timer_start;
	int door_delay = 1200 / ELEVATOR_SPEED; // 출입문 작동 지연

	inside.floor_button[cur_floor - 1] = 0;	// 해당 층 수 버튼 초기화
	elevator_print_to_screen();
	inside.open_button = 0;
	elevator_print_to_screen();
	printf("Door Opend!\n");
	// 출입문 열기 지연
	wait(door_delay);

	// 타이머 활성화
	timer_start = clock();

	while (1) {
		inside.open_button = 0;
		inside.close_button = 0;

		// 버튼 입력 활성화
		inside_button_press();

		// 열기 버튼이나 현재 층과 같은 버튼 눌리면 출입문 재개방
		if (inside.open_button || inside.floor_button[cur_floor - 1]) {
			inside.floor_button[cur_floor - 1] = 0;	// 해당 층 수 버튼 초기화
			elevator_print_to_screen();
			printf("Door Re-opend!\n");
			inside.open_button = 0;
			continue;
		}

		// 닫기 버튼 눌리면 출입문 닫음
		if (inside.close_button) {
			elevator_print_to_screen();
			printf("Force Door Closd!\n");
			inside.close_button = 0;
			// 출입문 닫기 지연
			wait(door_delay);
			return;
		}

		// 시간 만료되면 출입문 닫음
		// ELEVATOR_SPEED 가 1인 경우 3초 후 만료됨
		if (clock() - timer_start > 3 * CLOCKS_PER_SEC / ELEVATOR_SPEED) {
			elevator_print_to_screen();
			printf("Door Closd!\n");
			inside.close_button = 0;
			// 출입문 닫기 지연
			wait(door_delay);
			return;
		}
	}
}

// 엘리베이터 올라가기 함수
void elevator_up(void) {
	int i;

	// 엘리베이터가 정지해 있을때 열림 버튼에 대한 확인
	// 아래 for 문이 루프를 돌고 있는동안엔 이곳을 못오기 때문에
	// 엘리베이터가 정지해 있는 동안에만 입력을 확인함
	if(inside.open_button)
		elevator_door_open();

	for (i = cur_floor - 1; i < TOTAL_FLOORS; i++) {
#ifdef DEBUG_ELEVATOR
		printf("elevator_up() called\n");
#endif
		// 열기/닫기 버튼 비활성화 되지 않는 버그 수정
		inside.open_button = 0;
		inside.close_button = 0;
		elevator_print_to_screen();

		if (inside.floor_button[i]) {	// 버튼이 눌린 층을 발견하면
			elevator_door_open();		// 출입문 개방
		}

		// 남은 버튼이 있는지 확인
		// 올라가는 방향(is_up = 1)으로 더 이상 버튼 없으면 함수 종료
		if (!inside_button_check(cur_floor, 1))
			return;

		inside_button_press();	// 버튼 입력 대기 및 엘리베이터 이동 지연
		cur_floor++;
	}
}

// 엘리베이터 내려가기 함수
void elevator_down(void) {
	int i;

	// 엘리베이터가 정지해 있을때 열림 버튼에 대한 확인
	// 아래 for 문이 루프를 돌고 있는동안엔 이곳을 못오기 때문에
	// 엘리베이터가 정지해 있는 동안에만 입력을 확인함
	if (inside.open_button)
		elevator_door_open();

	for (i = cur_floor - 1; i >= 0; i--) {
#ifdef DEBUG_ELEVATOR
		printf("elevator_down() called\n");
#endif
		// 열기/닫기 버튼 비활성화 되지 않는 버그 수정
		inside.open_button = 0;
		inside.close_button = 0;
		elevator_print_to_screen();

		if (inside.floor_button[i]) {	// 버튼이 눌린 층을 발견하면
			elevator_door_open();		// 출입문 개방
		}

		// 남은 버튼이 있는지 확인
		// 내려가는 방향(is_up = 0)으로 더 이상 버튼 없으면 함수 종료
		if (!inside_button_check(cur_floor, 0))
			return;

		inside_button_press();	// 버튼 입력 대기 및 엘리베이터 이동 지연
		cur_floor--;
	}
}
