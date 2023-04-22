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

    // 人物初始位置，地图中央
    int player_x = map_width / 2 - player_width / 2;
    int player_y = map_height / 2 - player_height / 2;

    // 加载子弹图片
    IMAGE bulletImage;
    loadimage(&bulletImage, L"C:\\project\\cpp\\show\\resource\\篮球.png");//30*30
    int bullet_width = bulletImage.getwidth();//获取子弹宽度
    int bullet_height = bulletImage.getheight();//获取子弹高度
    float bullet_dir_x, bullet_dir_y;//方向坐标
    int bullet_x, bullet_y;//实际坐标
    int t = 0;//子弹飞行时间


    //定义结构体鼠标	
    ExMessage msg;
   
    while (true)
    {
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

        // 计算相机视角的偏移量，使得人物一直保持在视角的正中央
        CameraPos.x = player_x - WIDTH/2;
        CameraPos.y = player_y - HEIGHT/2;
        
        // 绘制背景图
        putimage(0, 0, WIDTH, HEIGHT, &map, CameraPos.x, CameraPos.y);

        // 绘制人物图
        //putimage(WIDTH / 2, HEIGHT / 2, &playerImg);//带背景
        drawAlpha(&playerImg, WIDTH / 2 - player_width / 2, HEIGHT / 2 - player_height / 2);//优化后没有背景
     
        //绘制子弹
        if (t > 0 && bullet_x >= 1530 && bullet_x <= 8670 && bullet_y >= 1020 && bullet_y <= 5100)
        {
            t--;
            drawbullet(bullet_x, bullet_y, CameraPos.x, CameraPos.y, &bulletImage);
            bullet_x += bullet_speed * bullet_dir_x;
            bullet_y += bullet_speed * bullet_dir_y;
        }
        else
        {
            t = -99;
            bullet_x = 0;
            bullet_y = 0;
            bullet_number = 1;
        }

        // 刷新界面
        // 清空鼠标消息缓存
        FlushMouseMsgBuffer();  
        flushmessage();

        // 稍微延迟一下，避免过快刷新
        Sleep(30);              
    }
    // 关闭图形环境
    closegraph();

    return 0;
}