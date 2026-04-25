#include "systems/rendering.h"
#include "components/camera.h"
#include "shared_unif_locations.h"
#include "ql_time.h"
#include <cmath>
#include <memory>

C3D_RenderTarget *ql::systems::Graphics::target[4];

static const int DISPLAY_TRANSFER_FLAGS = GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) |
			GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) |
			GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO);

void ql::systems::Graphics::Init() {
    // initialize libctru gfx driver, citro3d and citro2d
    gfxInitDefault();
   	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    
    #if DISPLAY_CONSOLE
        consoleInit(GFX_BOTTOM, NULL);
    #endif
    
    // initialize top display render buffers
    target[0] = C3D_RenderTargetCreate(240, 800, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8); // wide mode buffer
    target[1] = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8); // top left buffer
    target[2] = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8); // top right buffer
    
    // make the buffers overlap to save on memory
    // works fine since the two modes are mutually exclusive
    
    // initialize bottom display render buffer
    target[3] = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    
    // assign display outputs
    C3D_RenderTargetSetOutput(target[0], GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
    C3D_RenderTargetSetOutput(target[1], GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
    C3D_RenderTargetSetOutput(target[2], GFX_TOP, GFX_RIGHT, DISPLAY_TRANSFER_FLAGS);
    C3D_RenderTargetSetOutput(target[3], GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
}

void ql::systems::Graphics::DrawScene(std::unique_ptr<Scene>) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    // set time uniform
    C3D_FVUnifSet(GPU_VERTEX_SHADER, shared_unifs::time_loc, Time::curTime, sinf(Time::curTime), cosf(Time::curTime), Time::deltaTime);
    
    // render using all active cameras which render to render textures
    // scene.reg.view<Transform, Camera>().each([](auto entity, auto& pos, auto& cam) {
    //     if (cam.renderTex != nullptr) {
    //         // cam.Render(); 
    //     }
    // });
    
    // then render using active camera for each screen
    
    C3D_FrameEnd(GX_CMDLIST_FLUSH);
}

void ql::systems::Graphics::Exit() {
    // exit graphics
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    // we don't need to free the display render buffers since it's done by the system automatically
    // and it prevents double-freeing due to overlapping memory segments
}
