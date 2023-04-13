#include <main_state.h>
#include <glad/glad.h>
#include <math.h>


#include <rafgl.h>

#include <game_constants.h>

static rafgl_raster_t raster;
static rafgl_raster_t checker;

static rafgl_texture_t texture;
static rafgl_spritesheet_t hero;
static rafgl_spritesheet_t flame;
int i=0;


#define NUMBER_OF_TILES 3
rafgl_raster_t tiles[NUMBER_OF_TILES];
rafgl_raster_t numbers[10];


#define WORLD_SIZE 1280
int tile_world[WORLD_SIZE][WORLD_SIZE];

#define TILE_SIZE 64

static int raster_width = RASTER_WIDTH, raster_height = RASTER_HEIGHT;

static char save_file[256];
int save_file_no = 0;

int camx = 0, camy = 0;

int hero_pos_x = 50;
int hero_pos_y = 250;

int hero_speed = 1;
int jump_speed = 240;
int vert_speed = -40;
int r,r2;
int x_start;
int raz;
int k=0;
int flag=1;
int flame_animation=0;
int flame_direction=3;

int render_tilemap(rafgl_raster_t *raster)
{
    int x, y;
    int x0 = camx / TILE_SIZE-20;
    int x1 = x0 + (raster_width / TILE_SIZE) + 40;
    int y0 = camy / TILE_SIZE;
    int y1 = y0 + (raster_height / TILE_SIZE) + 2;

    if(x0 < 0) x0 = 0;
    if(y0 < 0) y0 = 0;
    if(x1 < 0) x1 = 0;
    if(y1 < 0) y1 = 0;

    if(x0 >= WORLD_SIZE) x0 = WORLD_SIZE - 1;
    if(x1 >= WORLD_SIZE) x1 = WORLD_SIZE - 1;

    rafgl_raster_t *draw_tile;
        i++;
        i%=182;
        if(i==30)
            r=rand()%300;
        if(i==121)
            r2=rand()%300;


        for(x = x0; x <= x1; x++)
        {
            draw_tile = tiles + 0;
            rafgl_raster_draw_raster(raster, draw_tile, x * TILE_SIZE - camx, 12*TILE_SIZE - camy - draw_tile->height + TILE_SIZE);
        }

        for(x = x0; x <= x1; x++)
        {
            if(x> 9){
                if(x%20==0){
                    draw_tile = tiles + 1;
                    rafgl_raster_draw_raster(raster, draw_tile, x * TILE_SIZE - camx, 17*TILE_SIZE- draw_tile->height + TILE_SIZE-r);
                    x_start=x;
                    raz=r;
                }
                else if(x%10==0){
                    draw_tile = tiles + 1;
                    rafgl_raster_draw_raster(raster, draw_tile, x * TILE_SIZE - camx, 17*TILE_SIZE - draw_tile->height + TILE_SIZE-r2);
                    x_start=x;
                    raz=r2;
                }
                if(x%20==0){
                    draw_tile = tiles + 2;
                    rafgl_raster_draw_raster(raster, draw_tile, x * TILE_SIZE - camx, 5*TILE_SIZE - draw_tile->height + TILE_SIZE-35-r);
                    x_start=x;
                }
                else if(x%10==0){
                    draw_tile = tiles + 2;
                    rafgl_raster_draw_raster(raster, draw_tile, x * TILE_SIZE - camx, 5*TILE_SIZE - draw_tile->height + TILE_SIZE-35-r2);
                    x_start=x;
                }
                int broj=camx/643;
                int k=0;
                if(camx<643){
                    draw_tile = numbers;
                    rafgl_raster_draw_raster(raster,draw_tile,400,100);
                }

                while(broj>0){
                    draw_tile = numbers + broj%10;
                    rafgl_raster_draw_raster(raster,draw_tile,400-k*24,100);
                    broj/=10;
                    k++;
                }

                if(hero_pos_x+camx+30>=x_start*TILE_SIZE&&hero_pos_x+camx<=x_start * TILE_SIZE +x1&&(hero_pos_y-510>=0-raz||hero_pos_y-338<=0-raz)){
                    return 0;
                }
            }

        }


    return 1;
}


void main_state_init(GLFWwindow *window, void *args)
{


    rafgl_raster_init(&raster, raster_width, raster_height);
    rafgl_spritesheet_init(&hero, "res/images/bird.png", 1, 1);
    rafgl_spritesheet_init(&flame, "res/images/flame.png", 3, 4);

    int i;

    char tile_path[256];


        sprintf(tile_path, "res/tiles/pozadina.jpg");
        rafgl_raster_load_from_image(&tiles[0], tile_path);
        sprintf(tile_path, "res/tiles/prepreka1.png");
        rafgl_raster_load_from_image(&tiles[1], tile_path);
        sprintf(tile_path, "res/tiles/prepreka2.png");
        rafgl_raster_load_from_image(&tiles[2], tile_path);
        for(i=0;i<10;i++)
        {
            sprintf(tile_path, "res/images/%d.png",i);
            rafgl_raster_load_from_image(&numbers[i], tile_path);
        }


    //init_tilemap();
    rafgl_texture_init(&texture);
}





void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{


    /* izmeni raster */

    int x, y;

    float xn, yn;

    rafgl_pixel_rgb_t sampled, sampled2, resulting, resulting2;

    if(flag){
        if (game_data->keys_down[RAFGL_KEY_SPACE])
         {
                vert_speed = jump_speed;
         }
         hero_pos_y -= vert_speed * delta_time;
         vert_speed -= 800 * delta_time;
         if(hero_pos_y<0)
         {
              hero_pos_y=0;
              vert_speed=-40;
         }

         if (hero_pos_y>640)
         {
                flag=0;
                return;
         }

        camx+=7;


        flag=render_tilemap(&raster);
        rafgl_raster_draw_spritesheet(&raster, &hero, 0, 0, hero_pos_x, hero_pos_y);
        rafgl_raster_draw_spritesheet(&raster, &flame, flame_animation, flame_direction, 0, hero_pos_y-7);
        flame_direction--;
        if(flame_direction==-1)
        {
            flame_direction=3;
            flame_animation++;
            if(flame_animation==3)
                flame_animation=0;
        }
        /* shift + s snima raster */
        if(game_data->keys_pressed[RAFGL_KEY_S] && game_data->keys_down[RAFGL_KEY_LEFT_SHIFT])
        {
            sprintf(save_file, "save.png");
            rafgl_raster_save_to_png(&raster, save_file);
        }



        /* update-uj teksturu*/

        rafgl_texture_load_from_raster(&texture, &raster);
    }
    else if(game_data->keys_pressed[RAFGL_KEY_SPACE])
    {
        flag=1;
        hero_pos_x = 50;
        hero_pos_y = 250;
        camx=0;
        camy=0;
        flame_animation=0;
        flame_direction=3;
        i=0;
    }


}


void main_state_render(GLFWwindow *window, void *args)
{
    /* prikazi teksturu */
    rafgl_texture_show(&texture);
}


void main_state_cleanup(GLFWwindow *window, void *args)
{
    rafgl_raster_cleanup(&raster);
    rafgl_texture_cleanup(&texture);

}
