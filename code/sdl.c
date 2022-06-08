#include <SDL2/SDL.h>

#include <stdio.h>

#define global_var static
#define internal static
#define persist static

#define WINDOW_WIDTH  1080
#define WINDOW_HEIGHT 720

#define RGBA_BLACK (0x000000ff)
#define RGBA_WHITE (0xffffffff)

#define BOOL32_FALSE ((uint)0)
#define BOOL32_TRUE ((uint)1)

typedef Uint32 bool32;

typedef Uint8  u8;
typedef Uint16 u16;
typedef Uint32 u32;
typedef Uint64 u64;

typedef Sint8  s8;
typedef Sint16 s16;
typedef Sint32 s32;
typedef Sint64 s64;

typedef float  f32;
typedef double f64;

internal void
CloseResources(SDL_Window* Window, SDL_Renderer* Renderer, SDL_PixelFormat* PixelFormat, SDL_Texture* Screen, u32* ScreenBuffer)
{
   if(Window) SDL_DestroyWindow(Window);
   if(Renderer) SDL_DestroyRenderer(Renderer);
   if(PixelFormat) SDL_FreeFormat(PixelFormat);
   if(Screen) SDL_DestroyTexture(Screen);
   if(ScreenBuffer) free(ScreenBuffer);
}

internal void
ChekcAndExitAtSDLError(u32 Error, SDL_Window* Window, SDL_Renderer* Renderer, SDL_PixelFormat* PixelFormat, SDL_Texture* Screen)
{
   if(Error)
   {
      fprintf(stderr, "[ERROR] %s", SDL_GetError());
      CloseResources(Window, Renderer, PixelFormat, Screen, NULL);
      exit(EXIT_FAILURE);
   }
}

int
main(int argc, char *argv[])
{
   ChekcAndExitAtSDLError(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER), NULL, NULL, NULL, NULL);

   SDL_Window* Window = SDL_CreateWindow("Title",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, 0);
   ChekcAndExitAtSDLError(!Window, Window, NULL, NULL, NULL);

   // TODO(Farid): Add VSYNC flag
   u32 RendererFlags = SDL_RENDERER_ACCELERATED;
   SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, RendererFlags);
   ChekcAndExitAtSDLError(!Renderer, Window, Renderer, NULL, NULL);

   SDL_PixelFormat* PixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
   ChekcAndExitAtSDLError(!PixelFormat, Window, Renderer, PixelFormat, NULL);

   SDL_Texture* Screen = SDL_CreateTexture(Renderer,
      PixelFormat->format,
      SDL_TEXTUREACCESS_STREAMING,
      WINDOW_WIDTH,
      WINDOW_HEIGHT);
   ChekcAndExitAtSDLError(!Screen, Window, Renderer, PixelFormat, Screen);

   u32* ScreenBuffer = (u32*)calloc(WINDOW_WIDTH * WINDOW_HEIGHT, sizeof(u32));
   if(!ScreenBuffer)
   {
      ChekcAndExitAtSDLError(BOOL32_TRUE, Window, Renderer, PixelFormat, Screen);
   }

   u32 Close = 0;
   while(!Close)
   {
      SDL_Event Event;
      while(SDL_PollEvent(&Event))
      {
         if(Event.type == SDL_QUIT || Event.key.keysym.scancode == SDL_SCANCODE_Q)
         {
            Close = 1;
         }
      }

      SDL_UpdateTexture(Screen, NULL, ScreenBuffer, WINDOW_WIDTH * sizeof(u32));
      SDL_RenderClear(Renderer);
      SDL_RenderCopy(Renderer, Screen, NULL, NULL);
      SDL_RenderPresent(Renderer);
   }

   CloseResources(Window, Renderer, PixelFormat, Screen, ScreenBuffer);

   exit(EXIT_SUCCESS);
}
