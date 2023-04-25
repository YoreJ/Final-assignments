#include "show.h"

int main()
{
    // 初始化窗口和图形环境
    initgraph(WIDTH, HEIGHT);
    setbkcolor(WHITE);               //用于设置当前绘图背景色
    cleardevice();                  //是用当前背景色清空屏幕，并将当前点移至 (0, 0)

    // 加载背景图
    IMAGE map;
    loadimage(&map, L"C:\\project\\cpp\\show\\resource\\map.png");//10200*6120
    int map_width = map.getwidth();//获取地图宽度
    int map_height = map.getheight();//获取地图高度

    // 加载人物图
    IMAGE playerImg;
    loadimage(&playerImg, L"C:\\project\\cpp\\show\\resource\\cxk.png");//60*60
    int player_width = playerImg.getwidth();//获取玩家宽度
    int player_height = playerImg.getheight();//获取玩家高度

    // 加载子弹图片及信息
    IMAGE bulletImage;
    loadimage(&bulletImage, L"C:\\project\\cpp\\show\\resource\\篮球.png");//30*30
    int bullet_width = bulletImage.getwidth();//获取子弹宽度
    int bullet_height = bulletImage.getheight();//获取子弹高度
    float bullet_dir_x, bullet_dir_y;//方向坐标
    int bullet_x = 0, bullet_y = 0;//实际坐标
    int t = 0;//子弹飞行时间

    // 加载敌人图片
    IMAGE xhzImg;
    loadimage(&xhzImg, L"C:\\project\\cpp\\show\\resource\\xhz.png");
    ikun xhz[xhz_number];

    //种子
    srand(time(0));

    // 加载初始界面
    IMAGE start;
    loadimage(&start, L"C:\\project\\cpp\\show\\resource\\kunstart.png");
    IMAGE m;
    loadimage(&m, L"C:\\project\\cpp\\show\\resource\\m.png");

    //加载结束界面
    IMAGE end;
    loadimage(&end, L"C:\\project\\cpp\\show\\resource\\end.png");

    //定义结构体鼠标
    ExMessage msg;
   
    //定义游戏是否继续
    bool con = true;

    //开始界面
    while (true)
    {
        putimage(0, 0, &start);
        peekmessage(&msg, EM_MOUSE);
        int mx = msg.x - 45;
        int my = msg.y - 45;
        if (mx > 0 && mx < WIDTH - 90 && my > 0 && my < HEIGHT - 90)
        {
            drawAlpha(&m, mx, my);
        }
        if (msg.message == WM_LBUTTONDOWN && msg.x >= 578 && msg.x <= 667 && msg.y >= 82 && msg.y <= 161)
        {
            break;
        }
        FlushMouseMsgBuffer();
        flushmessage();
        Sleep(30);
    }

    while(con)
    {
        //游戏内容初始化
        int player_speed = Player_speed;//人物速度
        int bullet_speed = Bullet_speed;//子弹速度
        int bullet_number = Bullet_number;//拥有子弹数量，目前不可变
        int bullet_range = Bullet_range;//子弹攻击范围
        int xhz_speed = Xhz_speed;//小黑子速度
        int HP = constHP;//初始血量
        hit = false;//判断是否命中
        mark = 0;//得分

        // 人物初始位置，地图中央
        int player_x = map_width / 2 - player_width / 2;
        int player_y = map_height / 2 - player_height / 2;

        //初始化敌人位置
        for (int i = 0; i < xhz_number; i++)
        {
            xhz[i].x = (rand() % 7140 + 1530);
            xhz[i].y = (rand() % 4080 + 1020);
        }


        //游戏主体
        while (HP > 0)
        {
            //-----------------------信息计算部分----------------------------
            // 根据wasd按键调整人物位置
            if (GetAsyncKeyState('W') & 0x8000)
                player_y -= player_speed;
            if (GetAsyncKeyState('A') & 0x8000)
                player_x -= player_speed;
            if (GetAsyncKeyState('S') & 0x8000)
                player_y += player_speed;
            if (GetAsyncKeyState('D') & 0x8000)
                player_x += player_speed;
            if (player_y <= 1020)//位置限定
                player_y = 1020;
            if (player_x >= 8670)
                player_x = 8670;
            if (player_y >= 5100)
                player_y = 5100;
            if (player_x <= 1530)
                player_x = 1530;

            //子弹方向与位置计算
            peekmessage(&msg, EM_MOUSE);
            if (msg.message == WM_LBUTTONDOWN && bullet_number > 0)
            {
                bullet_number--;
                float length = sqrt((msg.x - WIDTH / 2) * (msg.x - WIDTH / 2) + (msg.y - HEIGHT / 2) * (msg.y - HEIGHT / 2));
                bullet_dir_x = (msg.x - WIDTH / 2) / length;
                bullet_dir_y = (msg.y - HEIGHT / 2) / length;
                bullet_x = player_x;
                bullet_y = player_y;
                t = bullet_range / bullet_speed;//飞行时间
            }

            //小黑子方向与位置计算
            for (int i = 0; i < xhz_number; i++)
            {
                float length = sqrt((xhz[i].x - player_x) * (xhz[i].x - player_x) + (xhz[i].y - player_y) * (xhz[i].y - player_y));
                if (length <= 30)
                {
                    HP--;
                    xhz[i].x = (rand() % 7140 + 1530);
                    xhz[i].y = (rand() % 4080 + 1020);
                }
                if (sqrt((xhz[i].x - bullet_x) * (xhz[i].x - bullet_x) + (xhz[i].y - bullet_y) * (xhz[i].y - bullet_y)) <= 30)
                {
                    hit = true;
                    mark += 10;
                    xhz[i].x = (rand() % 7140 + 1530);
                    xhz[i].y = (rand() % 4080 + 1020);
                }
                xhz[i].dir_x = (xhz[i].x - player_x) / length;
                xhz[i].dir_y = (xhz[i].y - player_y) / length;
                xhz[i].x -= xhz[i].dir_x * xhz_speed;
                xhz[i].y -= xhz[i].dir_y * xhz_speed;
            }

            //-----------------------图形绘制部分----------------------
            // 计算相机视角的偏移量，使得人物一直保持在视角的正中央
            CameraPos.x = player_x - WIDTH / 2;
            CameraPos.y = player_y - HEIGHT / 2;

            // 绘制背景图
            putimage(0, 0, WIDTH, HEIGHT, &map, CameraPos.x, CameraPos.y);

            // 绘制人物图
            drawAlpha(&playerImg, WIDTH / 2 - player_width / 2, HEIGHT / 2 - player_height / 2);//优化后没有背景

            //绘制小黑子
            for (int i = 0; i < xhz_number; i++)
            {
                drawxhz(xhz[i].x, xhz[i].y, CameraPos.x, CameraPos.y, &xhzImg);
            }
            //绘制子弹
            if (t > 0 && bullet_x >= 1530 && bullet_x <= 8670 && bullet_y >= 1020 && bullet_y <= 5100 && !hit)
            {
                t--;
                drawbullet(bullet_x, bullet_y, CameraPos.x, CameraPos.y, &bulletImage);
                bullet_x += bullet_speed * bullet_dir_x;
                bullet_y += bullet_speed * bullet_dir_y;
                hit = false;
            }
            else
            {
                t = -99;
                bullet_x = 0;
                bullet_y = 0;
                bullet_number = 1;
                hit = false;
            }

            //信息面板
            settextstyle(30, 0, _T("黑体"));
            setbkmode(TRANSPARENT);
            _stprintf_s(MARK, _T("得分：%d"), mark);
            _stprintf_s(hp, _T("血量：%d"), HP);
            settextcolor(BLACK);
            outtextxy(0, 30, MARK);
            outtextxy(0, 0, hp);

            // 刷新界面
            // 清空鼠标消息缓存
            FlushMouseMsgBuffer();
            flushmessage();

            // 稍微延迟一下，避免过快刷新
            Sleep(25);
        }

        //结束界面
        putimage(0, 0, &end);
        setbkmode(TRANSPARENT);
        settextstyle(50, 0, _T("黑体"));
        _stprintf_s(MARK, _T("你的最终得分是：%d"), mark);
        outtextxy(400, 300, MARK);
        while (true)
        {
            peekmessage(&msg, EM_MOUSE);
            if (msg.message == WM_LBUTTONDOWN && msg.x < 200 && msg.y > 700)
            {
                break;
            }
            else if (msg.message == WM_LBUTTONDOWN && msg.x > 1000 && msg.y > 700)
            {
                con = false;
                break;
            }
            FlushMouseMsgBuffer();
            flushmessage();
            Sleep(20);
        }
    }

    // 关闭图形环境
    closegraph();

    return 0;
}