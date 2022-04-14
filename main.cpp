#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
int red=255,green=105,blue=180;

void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles(double BDR)
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BDR, -BUBBLE_DEFAULT_VX, 5, COLOR(red,green,blue)));
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BDR, BUBBLE_DEFAULT_VX, 5, COLOR(red,green,blue)));
    return bubbles;
}


int main()
{

    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);
     // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);
    int Level=1;

    string lvl="Level: 1";
    Text Lvl(50,125, lvl);
    Rectangle R1(50,125,90,30);
MAIN:

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd="CMD: ";
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
    //Rectangle R2(LEFT_MARGIN, BOTTOM_MARGIN,200,50);
    string score="Score: 0";
    Text Sc(450, 25, score);
    Rectangle R3(450, 25,90,30);
    string health="Health: 2";
    Text H(50,25, health);
    Rectangle R4(50, 25,90,30);



    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles(40);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;
    int sum=0;
    int h=2;
    // Main game loop
    while (true)
    {
        if(Level==1)
        {
            BUBBLE_RADIUS_THRESHOLD = 20;
            green=105;
            blue=180;
            //BUBBLE_DEFAULT_VX*=1;
        }
        else if(Level==2)
        {
            BUBBLE_RADIUS_THRESHOLD = 10;
            green=175;
            blue=105;
            //BUBBLE_DEFAULT_VX*=2;
        }
        else if(Level==3)
        {
            BUBBLE_RADIUS_THRESHOLD = 5;
            green=205;
            blue=50;
        }
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')//I tried giving arrow key controls but apparently they make the  shooter move even when not required
                shooter.move(STEP_TIME, true);
            else if(c=='K')
                shooter.rota(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c=='M')
                shooter.rota(STEP_TIME, false);
            else if(c == 'w'||c=='H')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);
        //we check collision between the bullets and the bubbles
        for (unsigned int i=0; i < bubbles.size(); i++)
        {
            for(unsigned int j=0; j<bullets.size(); j++)
            {
                if((abs(bullets[j].get_center_x()-bubbles[i].get_center_x())<(bubbles[i].get_radius()+(bullets[j].get_width()/2)))&&(abs(bullets[j].get_center_y()-bubbles[i].get_center_y())<(bubbles[i].get_radius()+(bullets[j].get_height()/2))))
                {
                    sum++;
                    //cout<<sum<<" ";
                    if(sum<=9)
                    {
                        char c=48+sum;
                        score[score.length()-1]=c;
                    }
                    if(sum>9)
                    {
                        char c=38+sum;
                        score[score.length()-1]=c;
                        score[score.length()-2]='1';

                    }
                    if(sum>19)
                    {
                        char c=28+sum;
                        score[score.length()-1]=c;
                        score[score.length()-2]='2';

                    }
                    if(sum>29)
                    {
                        char c=18+sum;
                        score[score.length()-1]=c;
                        score[score.length()-2]='3';

                    }

                    Sc.setMessage(score);
                    cout<<score<<endl;
                    if(bubbles[i].get_radius()==BUBBLE_RADIUS_THRESHOLD)
                    {
                        bubbles.erase(bubbles.begin()+i);
                        bullets.erase(bullets.begin()+j);
                    }
                    if(bubbles[i].get_radius()>BUBBLE_RADIUS_THRESHOLD)
                    {
                         bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2, -BUBBLE_DEFAULT_VX, 30, COLOR(red,green,blue)));
                         bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2, BUBBLE_DEFAULT_VX, 60, COLOR(red,green,blue)));
                         bubbles.erase(bubbles.begin()+i);
                         bullets.erase(bullets.begin()+j);
                    }
                }
            }
        }
        for(unsigned int i=0;i<bubbles.size();i++)
        {
            if((abs(bubbles[i].get_center_x()-shooter.get_body_center_x())<(bubbles[i].get_radius()+(shooter.get_body_width())))&&(abs(bubbles[i].get_center_y()-shooter.get_body_center_y())<(bubbles[i].get_radius()+(shooter.get_body_height()/2))))
            {
                h--;
                //cout<<h;
                char c=48+h;
                health[health.length()-1]=c;
                if(bubbles[i].get_radius()==BUBBLE_RADIUS_THRESHOLD)
                {
                    bubbles.erase(bubbles.begin()+i);
                    //bullets.erase(bullets.begin()+j);
                }
                if(bubbles[i].get_radius()>BUBBLE_RADIUS_THRESHOLD)
                {
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2, -BUBBLE_DEFAULT_VX, 10, COLOR(red,green,blue)));
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2, BUBBLE_DEFAULT_VX, 20, COLOR(red,green,blue)));
                    bubbles.erase(bubbles.begin()+i);
                    //bullets.erase(bullets.begin()+j);
                }
            }
            else if((abs(bubbles[i].get_center_x()-shooter.get_head_center_x())<(bubbles[i].get_radius()+shooter.get_head_radius()))&&(abs(bubbles[i].get_center_y()-shooter.get_head_center_y())<(bubbles[i].get_radius()+shooter.get_head_radius())))
            {
                h--;
                //cout<<h;
                char c=48+h;
                health[health.length()-1]=c;
                if(bubbles[i].get_radius()==BUBBLE_RADIUS_THRESHOLD)
                {
                    bubbles.erase(bubbles.begin()+i);
                    //bullets.erase(bullets.begin()+j);
                }
                if(bubbles[i].get_radius()>BUBBLE_RADIUS_THRESHOLD)
                {
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2, -BUBBLE_DEFAULT_VX, 10, COLOR(red,green,blue)));
                    bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2, BUBBLE_DEFAULT_VX, 20, COLOR(red,green,blue)));
                    bubbles.erase(bubbles.begin()+i);
                    //bullets.erase(bullets.begin()+j);
                }
            }
            H.setMessage(health);
        }
        if(h==0)
        {
            Text Final(250,150,"LOSE");
            Final.imprint();
            break;
        }
        if(bubbles.size()==0)
        {
            if(Level>=3)
            {
                Text Final(250,150,"WIN");
                Final.imprint();
                green=105;
                blue=180;
                break;
                wait(5);
            }
            else
            {
                Level++;
                char c=48+Level;
                lvl[lvl.length()-1]=c;
                cout<<lvl<<endl;
                Lvl.setMessage(lvl);
                goto MAIN;
            }
        }
    }
    string c="9";
    //Text Count_Down(250,250,c);
    Rectangle RP(250,400,200,50);
    string Rp="Play Again";
    Text rp(250,400,Rp);
    {
        int i=10;
        repeat(10)
        {
            i--;
            char Ch=48+i;
            c[c.length()-1]=Ch;
            //Count_Down.setMessage(c);
            int w=getClick();
            int x = w/65536, y = w % 65536;
            if(x>150&&x<350&&y>375&&y<425)
            {
                Text Final(250,150,"                                            ");
                Final.imprint();
                for(unsigned int j=0;j<bubbles.size();j++)
                {
                    bubbles.erase(bubbles.begin()+j);
                }
                for(unsigned int j=0;j<bullets.size();j++)
                {
                    bullets.erase(bullets.begin()+j);
                }
                Level=1;
                char c=48+Level;
                lvl[lvl.length()-1]=c;
                cout<<lvl<<endl;
                Lvl.setMessage(lvl);
                goto MAIN;
            }
            wait(1.1);
        }
    }
    wait(2);
}
