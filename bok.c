#include <stdio.h>  
#include <stdlib.h> 
#include <time.h> 
#include <termio.h>
#include <unistd.h>     //256 608 sleep(1)
#include <ctype.h>
#define YY 20
#define XX 60
void show_line(char ch, int num);               // 줄                             
void enter(void);                               // 개행 문자 외 버퍼에 남는 문자 제거
void cal_exponent(int a, int* b);               // a의 자리수를 b에 저장하는 함수
int getch(void);                                // window에서 사용가능한 getch를 쓰기 위해 함수 전방선언

int main(void)
{
    srand(time(NULL));                          // 랜덤숫자 seed 초기화 

    int hp = 3000;                              // 유저 현재 체력
    int hp_max = 3000;                          // 유저 최대 체력
    int level = 1;                              // 레벨
    int monster_hp;                             // 몬스터 체력
    int result_battle;                          // 전투 결과
    float randnum;                              // 랜덤숫자
    int monster_randnum;                        // 몬스터 등장 여부
    int temp_hp;                                // 포션 회복 시 변화된 체력을 보여주기 위해 임시저장
    int half_use = 0;                           // 절대무적 아이템 사용 여부 확인
    int same;                                   // 배열안의 값과 다른 값 비교시 사용
    int item [10][1] = {                        // 아이템창 0번 회복 1번 데미지 감소 2번 폭탄 3번 도망
        {3},{2},{5},{1},
    };
    int input;                                  // 전투시 아이템 목록 선택 입력
    int input1;                                 // 아이템 선택 후 사용 여부 선택 입력
    int input2;                                 // 맵에서 아이템 사용 선택 입력
    int y = 0;                                  // 맵 y
    int x = 0;                                  // 맵 x
    char map[YY][XX] = {{' '}};                 // 맵 이중 배열
    char user = '@';                            // 맵 상 이동 표시 캐릭터?
    char key;                                   // wasd 입력 값

    for(y=0; y<YY; y++){                        // 맵 이중배열 생성
        for(x=0;x<XX;x++){
            if(y==0 || y==YY-1)
                map[y][x] = '-';
            else if (x==0 || x==XX-1)
                map[y][x] = '|';
            else if (y==12 && x==12)
                map[y][x] = user;
            else
                map[y][x] = ' ';
        }
    }

    char name [27][10] = {                      // 반이름 배열 생성
        {"유시온"},{"김승수"},{"권철민"},{"안광민"},{"강진영"},
        {"김영곤"},{"박선후"},{"김건"},{"이준호"},{"이철"},{"이동준"},
        {"황은비"},{"조세빈"},{"김성근"},{"이은승"},{"박희정"},{"박장미"},
        {"김민아"},{"조대정"},{"김재신"},{"박민건"},{"임석현"},{"황운하"},
        {"노주영"},{"김혜빈"},{"서훈"},{"오은지"}
    };

    char bok [10] = {"복이"};                   // 복이
    char username [10];                         // 유저 이름

    while(1)
    {    
        printf("이름을 입력해주세요: ");
        scanf("%s", username);                  // 유저 이름 입력
        enter();

        for (int i = 0; i < 27; i++)            // 유저 이름이 반이름 배열에 같은 값 있는지 확인
        {
            same = 0;
            for (int j = 0; j < 10; j++)
            {
                if (name[i][j]==username[j])
                {
                    ++same;
                }
            }
            if (same == 10)                     // 유저 이름이 반이름 배열에 있다면 복이로 교체
            {
                for (int k = 0; k < 10; k++)
                    name[i][k] = bok[k];
                break;                          // 교체완료 시 for문 나가기
            }
        }
        if (same == 10)
            break;                              // while문 나가기
        printf("이름을 다시 입력하여 주시길 바랍니다.\n");
    }
                                //시작화면 출력
    while(1)
    {
        for(y=0; y<YY; y++){                    // 맵 출력
            for(x=0;x<XX;x++)
                printf("%c",map[y][x]);
            printf("\n");
        }
        printf("wasd로 이동 아이템창 i\n");
        key = getch();                          // w, a, s, d로 만 상하좌우 이동
        while(key != 'a' && key != 's' && key != 'd' && key != 'w'&& key != 'i'){
            key = getch();
        }
        if(key =='i')                           // 맵에서 i 누르면 아이템 사용
            {
                printf("사용할 아이템을 골라주세요.\n");
                printf("1. 체력포션\n");
                printf("2. 절대무적\n");
                printf("3. 돌아가기\n");
                scanf("%d", &input2);
                enter();
                switch (input2)
                {
                    case 1:                     // 체력포션
                    {
                        printf("체력 포션은 체력을 30%%~80%% 회복합니다.\n");
                        printf("소지량: %d\n",item[0][0]);
                        printf("사    용 >>>1\n");
                        scanf("%d",&input1);
                        if (input1 == 1)
                        {
                            if (item[0][0] <= 0)
                            {
                                printf("체력 포션이 없습니다.\n");
                                continue;
                            }
                            randnum = (rand() % 51) + 30;                          // 30 ~ 80 수치 = ((0 ~ 50) + 30)
                            temp_hp = hp;                                          // 회복 전 유저의 현재 체력을 복사
                            hp += (int)((float)hp_max * ((float)randnum/100));     // 유저의 현재 체력 증가
                            if (hp > hp_max)                                       // 최대 체력 이상 회복 시, 최대 체력으로 제한
                            {
                                hp = hp_max;
                            }
                            printf("포션을 사용하였습니다.\n");
                            printf("%d의 체력을 회복하여 현재 체력 %d이 되었습니다.\n", hp - temp_hp, hp);    // 회복 후 체력 표시
                            item[0][0]--;                                           // 포션 개수 감소
                        }
                        else
                        {
                        continue;
                        } 
                        break;
                    }
                    case 2:                     // 절대무적
                    {
                        printf("절대무적 아이템은 다음 전투동안 데미지를 50%%감소시킵니다.\n");
                        printf("소지량: %d\n",item[1][0]);
                        printf("사용>>>1\n");
                        scanf("%d",&input1);
                        if (input1 == 1)
                        {
                            if (item[1][0] <= 0)
                            {
                                printf("절대무적 아이템이 없습니다.\n");
                                continue;
                            }
                            half_use = 1;                                               // 절대무적 아이템 사용
                            item[1][0]--;
                            break;
                        }
                        else
                        {
                        continue;
                        } 
                        break;
                    }
                    default :
                    break;
                }
                continue;
            }

        for(y=0; y<YY; y++){  
            for(x=0;x<XX;x++){
                if (map[y][x]==user){
                    if(key =='a' && x > 4){                                         // a누르면 이동
                        map[y][x]=' ';
                        map[y][x-3]=user;
                        break;
                    }
                    else if(key =='s' && y < YY-2){                                 // s 누르면 이동
                        map[y][x]=' ';
                        map[++y][x]=user;
                        break;
                    }
                    else if(key =='d' && x < XX-5){                                 // d 누르면 이동
                        map[y][x]=' ';
                        map[y][x+3]=user;
                        break;
                    }
                    else if(key =='w' && y > 1){                                    // w누르면 이동
                        map[y][x]=' ';
                        map[y-1][x]=user;
                        break;
                    }
                    }
                }
            }
        
        /* 몬스터 조우 했나요? */ 
        monster_randnum = rand()%2;                                                 // 몬스터 등장 여부
        result_battle = 3;
        randnum = rand()%27;                
        char monster_name [10];                                                     // 몬스터 이름 생성
        if (level == 5)                                                             // 전투에서 5번 이기면 복이와 전투
        {
            for (int i = 0; i < 10; i++)
                monster_name[i] = bok[i];
        }
        else
        {
            for (int i = 0; i < 10; i++)
                monster_name[i] = name[(int)randnum][i];                             // 몬스터 랜덤 생성
        }
        
        same = 0;
        for (int i = 0; i < 10; i++)
        {
            if(monster_name[i] == bok[i])                                            // 몬스터이름이 복이 인지 확인
                same++;
        }

        if (same == 10)
            randnum = rand()%4 + 5;                                                   // 복이체력 유저 최대체력 * (5~8배)
        else
            randnum = rand()%3 + 1;                                                   // 몬스터 체력 유저 최대체력 * (1~3배)    
        monster_hp = hp_max * randnum;

        int monster_hp_max = monster_hp;

        if (monster_randnum == 1)                                                   //몬스터 등장시
        {
            for(y=0; y<YY; y++){                                                    //맵 출력
                for(x=0;x<XX;x++)
                     printf("%c",map[y][x]);
                printf("\n");
            }

            printf("야생의 %s(을/를) 만났습니다.\n", monster_name);
            sleep(1);

            while (1)
            {
                int attack_user;                                                    // 유저 공격력
                int attack_mon;                                                     // 몬스터 공격력
                float randnum_m;                                                    // 몬스터 공격력
                int flag = 3;                                                       // 도망 선택 시 체크
                int choice;                                                         // 전투 시 유저 선택 값
                int exponent = 0;                                                   // 상태창 자리수 초기화 필요.
                int* pexponent = &exponent;                                         // 자리수 의 주소
                cal_exponent(monster_hp_max, pexponent);
                result_battle = 4;

                // 상태창 표시 - 레벨과 라운드 표시할 지 논의 필요.
                show_line('-',XX);                                                  // 구분선 표시
                printf(" %s\n", username);
                printf(" %d / %d\n", hp, hp_max);                                   // 유저 체력
                printf("%*s \n", XX + 2, monster_name);
                printf("%*d / %d\n", XX+(exponent-4)-2*exponent, monster_hp, monster_hp_max);   // 몬스터 체력
                
                show_line('-',XX);                                                  // 구분선 표시
                
                printf("%*s %*s %*s \n", XX -50, "1) 싸우다",  XX-60, "2) 아이템", XX-60, "3) 도망치기");
                show_line('-',XX);                      // 전투창 선택 보기 창
                
                printf("  %-63s \n", "행동을 선택하시오: ");        // 행동 선택 지시문
                scanf("%d", &choice);                              // 행동 입력
                show_line('-',XX);

                switch(choice)                                     // 입력받은 숫자에 따른 행동
                {
                    case 1 :                                       // 1.싸우다
                    {
                        randnum = (rand() % 4) + 1;                 // 1 ~ 4 = (0 ~ 3) + 1
                        attack_user = (int)hp * 0.5 * randnum;      // 유저의 남은 체력 * 0.5 * 랜덤숫자
                        /*  테스트 용 확률              */
                        // 유저는 몬스터에게 숫자만큼의 데미지를 입혔다
                        printf("%s(은/는) %s에게 %d의 데미지를 입혔다.\n", username, monster_name, attack_user);
                        monster_hp -= attack_user;                  // 몬스터 체력 감소
                        flag = 1;                                   // 도망 체크 = 1(전투 진행)
                        break;
                    }
                    case 2 :                                        // 2. 아이템
                    {
                        printf("\n사용할 아이템을 골라주세요.\n");
                        printf("1. 체력포션\n");
                        printf("2. 절대무적\n");
                        printf("3. 폭탄\n");
                        printf("4. 도망포션\n");
                        scanf("%d", &input);
                        switch (input)
                        {
                            case 1:                                 // 체력포션
                            {
                                printf("체력 포션은 체력을 30%%~80%% 회복합니다.\n");
                                printf("소지량: %d\n",item[0][0]);
                                printf("사용>>>1\n");
                                scanf("%d",&input1);
                                if (input1 == 1)
                                {
                                    if (item[0][0] <= 0)
                                    {
                                        printf("체력 포션이 없습니다.\n");
                                        continue;
                                    }
                                    randnum = (rand() % 51) + 30;           // 30 ~ 80 수치 = ((0 ~ 50) + 30)
                                    temp_hp = hp;                           // 회복 전 유저의 현재 체력을 복사
                                    hp += (int)((float)hp_max * ((float)randnum/100));          // 유저의 현재 체력 증가
                                    if (hp > hp_max)                        // 최대 체력 이상 회복 시, 최대 체력으로 제한
                                    {
                                        hp = hp_max;
                                    }
                                    printf("포션을 사용하였습니다.\n");
                                    printf("%d의 체력을 회복하여 현재 체력 %d이 되었습니다.\n", hp - temp_hp, hp);    // 회복 후 체력 표시
                                    item[0][0]--;
                                    flag = 1;                                                   // 도망 체크 = 1(전투 진행)
                                }
                                else
                                {
                                continue;
                                } 

                                break;
                            }
                            case 2:                                     // 절대무적
                            {
                                printf("절대무적 아이템은 남은 전투동안 데미지를 50%%감소시킵니다.\n");
                                printf("소지량: %d\n",item[1][0]);
                                printf("사용>>>1\n");
                                scanf("%d",&input1);
                                if (input1 == 1)
                                {
                                    if (item[1][0] <= 0)
                                    {
                                        printf("절대무적 아이템이 없습니다.\n");
                                        continue;
                                    }
                                    half_use = 1;
                                    item[1][0]--;
                                    break;
                                }
                                else
                                {
                                continue;
                                } 
                                break;
                            }
                            case 3:                                     // 폭탄
                            {
                                printf("폭탄은 적 최대 체력의 50%% 데미지를 입힙니다.\n");
                                printf("소지량: %d\n",item[2][0]);
                                printf("사용>>>1\n");
                                scanf("%d",&input1);
                                if (input1 == 1)
                                {
                                    if (item[2][0] <= 0)
                                    {
                                        printf("폭탄이 없습니다. \n");
                                        continue;
                                    }
                                    monster_hp -= monster_hp_max/2;
                                    item[2][0]--;
                                    break;
                                }
                                else
                                {
                                continue;
                                } 
                                break;
                            }
                            case 4:                                        // 도망100%
                            {
                                printf("도망 포션은 사용 시 100%% 도망갈 수 있습니다.\n");
                                printf("소지량: %d\n",item[3][0]);
                                printf("사용>>>1\n");
                                scanf("%d",&input1);
                                if (input1 == 1)
                                {
                                    if (item[3][0] <= 0)
                                    {
                                        printf("도망 포션이 없습니다.\n");
                                        continue;
                                    }
                                    flag = 0;
                                    item[3][0]--;
                                    break;
                                }
                                else
                                {
                                continue;
                                } 
                                break;
                            }
                            default:
                            {
                                continue;
                            }
                            break;
                        }
                        break;
                    }
                    case 3 :                                         // 3. 도망 (랜덤 확률로 가능)
                    {
                        randnum = rand() % 100;                      // 0 ~ 99
                        if (randnum < 30)                            // 0 ~ 29
                        {
                            flag = 0;                                // 도망체크 성공(flag = 0)
                        }
                        else                                         // 도망실패시
                        {
                            printf("도망에 실패하였습니다.\n");
                            flag = 1; 
                        }
                        break;
                    }
                    default :
                    {
                        printf("잘못된 입력입니다. 다시 입력해주세요\n"); 
                        scanf("%d", &choice);
                        enter();
                        continue;
                    }
                }

                if (flag == 0)                             // flag확인(도망 성공 시) 함수 종료
                {
                    printf("도망에 성공하였습니다.\n");
                    result_battle = 3;                     // 반환할 리절트 값 3
                    break;
                }
                if (monster_hp <= 0)                       // 적체력 확인 다운 시 함수 종료
                {
                    printf("전투에서 승리하였습니다.\n");
                    result_battle = 1;                     // 반환할 리절트 값 3
                    break;
                }
                else
                {
                    if (half_use==0)
                    {
                        randnum = rand() % 100;
                        if (randnum == 0)
                        {
                            randnum_m = (float)(rand() % 11 + 20) / 10;
                        }
                        if (randnum > 0 && randnum < 6)
                        {
                            randnum_m = (float)(rand() % 10 + 10) / 10;
                        }
                        else
                        {
                            randnum_m = (float)(rand() % 5 + 5) / 10;
                        }
                        attack_mon = (int)(hp_max *  randnum_m);         // (강제변환) 유저의 최대체력 * 0.5 ~ 3.0
                        hp = hp - attack_mon;
                        printf("%s(은/는) %s에게 %d의 데미지를 입혔습니다.\n", monster_name, username, attack_mon);
                    }
                    else
                    {
                        printf("절대무적 아이템을 사용했습니다.\n");
                        randnum = rand() % 100;
                        if (randnum == 0)
                        {
                            randnum_m = (float)(rand() % 11 + 20) / 10;
                        }
                        if (randnum > 0 && randnum < 6)
                        {
                            randnum_m = (float)(rand() % 10 + 10) / 10;
                        }
                        else
                        {
                            randnum_m = (float)(rand() % 5 + 5) / 10;
                        }
                        attack_mon = (int)(hp_max *  randnum_m);         // (강제변환) 유저의 최대체력 * 0.5 ~ 3.0
                        hp = hp - attack_mon/2;
                        printf("%s(은/는) %s에게 %d의 데미지를 입혔습니다.\n", monster_name, username, attack_mon/2);
                    }
                }
                                
                if (hp <= 0)                            // 유저 다운 체크
                {
                    show_line('-',XX);
                    printf(" %s\n", username);
                    if(hp<0)
                        printf(" %d / %d\n", 0, hp_max);
                    else
                        printf(" %d / %d\n", hp, hp_max);
                    printf("%*s \n", XX + 2, monster_name);
                    printf("%*d / %d\n", XX+(exponent-4)-2*exponent, monster_hp, monster_hp_max);
                    show_line('-',XX);

                    result_battle = 2;                     // 반환할 리절트 값 2(배드엔딩)
                    break;
                }
                else                                        // 유저 다운이 아니면 게임진행
                    continue;
            }
        }
        else                    // 맵에서 적을 만나지 않으면 절대무적 1개 지급
        {
            if(item[1][0] > 10) // 10개까지만 수용 가능
                continue;
            else
            {
                item[1][0]++;
                printf("몬스터를 만나지 않았습니다. 절대무적 1개 지급\n");
            }
        }
                                                       //전투 끝
        half_use = 0;

        if (same == 10 && result_battle == 1) //복이를 처치시 승리
        {
            break;
        }

        if (result_battle == 1)        //몬스터 전투 승리시
        {
            randnum = rand()% 11;      //회복포션 지급
            item[0][0] += randnum;
            printf("회복포션 %d개 획득\n", (int)randnum);
            if (item[0][0] > 300)       //  회복포션 300개까지
            {
                item[0][0] = 300;
            }
            randnum = rand()% 100;
            if (item[1][0] == 11) //무적포션 5퍼센트 지급
            {
                item[1][0]--;
            }
            printf("도망포션 : 1개 획득\n");
            item[3][0]++;                   // 도망 1개 지급
            randnum = rand()%131 + 120;
            hp_max *= randnum * 0.01;       // 최대체력 증가
            randnum = rand()%71 + 30;
            hp += hp_max * (randnum * 0.01);  // 최대체력 증가;
            level++;                    // 레벨 증가
            if (hp > hp_max)
            {
                hp = hp_max;
            }
            printf("\nLevel Up!   현재 Lv. %d 입니다.\n", level);
            printf("최대 체력이 증가했습니다. 현재 체력 %d 입니다.\n", hp); 
        }
        else if (result_battle == 2)    //패배시
        {
            break;
        }
        sleep(1);
    }
    
    if (result_battle == 1)
    {
        printf("승리\n");
    }
    if (result_battle == 2)
    {
        printf("패배\n");
    }
    
   return 0;
}

int getch(void)
{
    int ch;
    struct termios buf, save;
    tcgetattr(0,&save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);

    return ch;
}

void cal_exponent(int a, int* b)        // 입력한 숫자의 자리 수를 구하는 함수.
{
    int c;
    c = a / 10;
    (*b)++;
    if (c >= 10)
    {
        cal_exponent(c, b);
    }
}

void enter(void)                        // 개행 문자 외 버퍼에 남은 쓰레기 제거
{
    while (getchar() != '\n')
        {
            continue;
        }     
}

void show_line(char ch, int num)
{
    int i;

    for(i = 0; i < num; i++)
    {
        putchar(ch);
    }
    putchar('\n');
}