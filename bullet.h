#include <simplecpp>

class Bullet
{
private:
    Rectangle rect; // the rectangle representing the bullet
    double v;
    double ang; // velocity in y direction

public:
    Bullet(double center_x, double center_y, double dir, double width=5, double height=5, double v_=400, Color color=COLOR(255,0, 0)) {
        // Bullet constructor
        rect = Rectangle(center_x, center_y, width, height);
        rect.setColor(color);
        rect.setFill(true);
        v = v_;
        ang=dir;
    }

    bool nextStep(double t)
    {
        // move the bullet
        double vx=v*cosine(((ang*180.0)/3.141592)+90);
        double vy=v*sine(((ang*180.0)/3.141592)+90);
        double new_x = rect.getX() - vx*t;
        double new_y = rect.getY() - vy*t;
        if(new_y < (0-rect.getHeight()/2.0)|| new_y > (450+rect.getHeight()/2.0))   // out of canvas, so delete it
        {
            return false;
        }
        if(new_x < (0-rect.getWidth()/2.0)|| new_x > (500+rect.getHeight()/2.0))   // out of canvas, so delete it
        {
            return false;
        }
        rect.moveTo(new_x, new_y);
        return true;
    }

    double get_width()
    {
        // return the width of the bullet
        return rect.getWidth();
    }

    double get_height()
    {
        // return the height of the bullet
        return rect.getHeight();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the bullet
        return rect.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the bullet
        return rect.getY();
    }

};
