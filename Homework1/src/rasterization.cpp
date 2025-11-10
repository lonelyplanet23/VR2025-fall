#include "rasterization.h"
void swap_int(int &x, int &y)
{
    int temp = x;
    x = y;
    y = temp;
}
void draw_line_dda(Pixel start, Pixel end, std::vector<Pixel>& pixels) {
    int dx = end.x - start.x;
    int dy = end.y - start.y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float x_inc = dx / static_cast<float>(steps);
    float y_inc = dy / static_cast<float>(steps);
    float x = start.x;
    float y = start.y;

    for (int i = 0; i <= steps; i++) {
        pixels.emplace_back(x, y);
        x += x_inc;
        y += y_inc;
    }
}

void draw_line_bresenham(Pixel start, Pixel end, std::vector<Pixel>& pixels) {
    // TODO: 1.1 complete bresenham algorithm

    //基于dda的改进，引入误差，且只需要整数运算   
    int dx = end.x - start.x; // 一定大于0
    int dy = end.y - start.y;
    
    //比dda不同的地方，计算精确误差（和格子中点的比较）p0 = 2|Δy| - |Δx|
    
    int x = start.x;
    int y = start.y;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    bool steep = (abs(dy) > abs(dx)); //斜率大于1
    int x_step = (dx > 0) ? 1 : -1; //x增量
    int y_step = (dy > 0) ? 1 : -1; //y增量

    pixels.emplace_back(x,y);
    //不陡峭
    if(!steep)
    {
        int P = 2*abs(dy) - abs(dx);
        for(int i = 0; i <= steps; i++, x += x_step)
        {
            if(P < 0)
            {
                pixels.emplace_back(x+x_step, y);
                P = P + 2*abs(dy);
            }
            else
            {
                y += y_step;
                pixels.emplace_back(x+x_step, y);
                P = P + 2*abs(dy) - 2*abs(dx);
            }
        }
        return;
    }
    //陡峭
    //!互换xy地位，包括误差函数
    int P = 2*abs(dx) - abs(dy);
    for(int i = 0; i <= steps; i++, y += y_step)
    {
        if(P < 0)
        {
            pixels.emplace_back(x, y+y_step);
            P = P + 2*abs(dx);
        }
        else
        {
            x += x_step;
            pixels.emplace_back(x, y+y_step);
            P = P + 2*abs(dx) - 2*abs(dy);
        }
    }

    
}

void draw_ellipse(Pixel start, Pixel end, std::vector<Pixel>& pixels) {
    // TODO: 1.2 complete rasterization for ellipse 根据角点
    //core_point: 计算dy/dx, 根据中点在椭圆内（上）还是椭圆外 选择 T (xi+1, yi) or S (xi+1, yi-1) 
    //initiating the starting point and the decision parameter
    int a = abs(end.x - start.x) / 2; //半长轴
    int b = abs(end.y - start.y) / 2; //半短轴

    Pixel center((start.x + end.x) / 2, (start.y + end.y) / 2); //椭圆中心
    int x = 0;
    int y = b;
    
    
    double a2 = a * a;
    double b2 = b * b;

     
    //region 1 斜率大于-1（0 ~ -1）
    double p1 = b2 - a2 * b + 0.25 * a2;
    //!bug 数学问题 dy/dx = -b2*x/(a2*y) 所以应当判断 (2 * a2 * y)  != 0
    while((b2 * x <= a2 * y) && (2 * a2 * y) != 0) // dy/dx >-1
    {
        //画左右对称的四个点
        pixels.emplace_back(x + center.x, y + center.y);
        pixels.emplace_back(center.x - x, y + center.y);
        pixels.emplace_back(center.x - x, center.y - y);
        pixels.emplace_back(x + center.x, center.y - y);

        //更新下一个点和 中点方程参数 pi=f((xi+1),yi−0.5)
        if(p1 < 0) //在椭圆内 画(x+1,y)
        {
            p1 += b2 * (2 * x + 3);
        }
        else//在椭圆外 画(x+1,y-1)
        {
            p1 += b2 * (2 * x + 3) + a2 * (-2 * y + 2);
            y--;
        }
        x++;
    }
    //初始化p2 现在位于椭圆与y=x交点
    double p2 = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;
    //region 2 斜率小于-1
    while (y >= 0)
    {
        //画左右对称的四个点
        pixels.emplace_back(x + center.x, y + center.y);
        pixels.emplace_back(center.x - x, y + center.y);
        pixels.emplace_back(center.x - x, center.y - y);
        pixels.emplace_back(x + center.x, center.y - y);
        //更新下一个点和参数 与region1 xy互换
        if(p2 > 0) //在椭圆外 画(x,y-1)
        {
            p2 += a2 * (-2 * y + 3);
        }
        else//在椭圆内 画(x+1,y-1)
        {
            p2 += b2 * (2 * x + 2) + a2 * (-2 * y + 3);
            x++;
        }
        y--;
    }

}
