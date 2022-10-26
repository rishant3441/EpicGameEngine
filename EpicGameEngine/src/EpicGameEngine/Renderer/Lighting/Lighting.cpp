//
// Created by Rishan Thangaraj on 8/23/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/Renderer/Lighting/Lighting.h>
#include <EpicGameEngine/Renderer/Renderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glew.h>

namespace EpicGameEngine
{
    void Lighting::Init()
    {
        GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
        lightingTexture = GPU_CreateImage(Renderer::GetTarget()->w, Renderer::GetTarget()->h, GPU_FORMAT_RGBA);
        lightingTarget = GPU_LoadTarget(lightingTexture);
        lightingTarget = lightingTexture->target;
        GPU_EnableCamera(lightingTarget, false);
        GPU_SetCamera(lightingTarget, nullptr);

        shaderObj = GPU_CreateShaderProgram();

        vertexShader = "#version 110\n"
                       "uniform mat4 u_MVPMatrix;      // A constant representing the combined model/view/projection matrix.\n"
                       " \n"
                       "in vec4 a_Position;     // Per-vertex position information we will pass in.\n"
                       "varying out vec2 v_vTexcoord;\n"
                       " \n"
                       "// The entry point for our vertex shader.\n"
                       "void main()\n"
                       "{\n"
                       "    // gl_Position is a special variable used to store the final position.\n"
                       "    // Multiply the vertex by the matrix to get the final point in normalized screen coordinates.\n"
                       "    gl_Position = u_MVPMatrix * a_Position;\n"
                       "}";

        fragmentShader = "#version 410\n"
                         "layout(location = 0) out vec4 fragColor;\n"
                         "uniform vec3 lightpos;\n"
                         "uniform vec4 lightColor;\n"
                         "uniform vec4 lightIntensity;\n"
                         "uniform mat4 u_MVPMatrix;\n"
                         "void main()\n"
                         "{\n"
                         "fragColor = lightColor * 1.0/distance(lightpos, vec3(gl_FragCoord.x, gl_FragCoord.y, lightpos.z)) * lightIntensity/100;\n"
                         "}";
        shader = GPU_CompileShader(GPU_FRAGMENT_SHADER, fragmentShader.c_str());
        vShader = GPU_CompileShader(GPU_VERTEX_SHADER, vertexShader.c_str());

        Debug::Log::LogInfo("{}", GPU_GetShaderMessage());

        GPU_AttachShader(shaderObj, shader);
        GPU_AttachShader(shaderObj, vShader);
        GPU_LinkShaderProgram(shaderObj);
        glValidateProgram(shaderObj);
        GPU_DetachShader(shaderObj, shader);
        GPU_DetachShader(shaderObj, vShader);
    }

    void Lighting::Shutdown()
    {
        GPU_FreeImage(lightingTexture);
    }

    void Lighting::Render()
    {
        GPU_Flip(lightingTarget);
        lightingTexture->w = Renderer::GetTarget()->w;
        lightingTexture->h = Renderer::GetTarget()->h;
        GPU_SetBlending(lightingTexture, true);
        GPU_SetBlendMode(lightingTexture, GPU_BLEND_ADD);
        GPU_SetImageFilter(lightingTexture, GPU_FILTER_LINEAR_MIPMAP);
        GPU_Rect rect;
        GPU_SetActiveTarget(Renderer::viewportTarget);
        GPU_BlitRect(lightingTexture, nullptr, Renderer::viewportTarget, nullptr);
        GPU_DeactivateShaderProgram();
    }

    void Lighting::RenderAt(glm::vec3 position, float intensity)
    {
        GPU_SetActiveTarget(Renderer::target);
        glm::mat4 proj = glm::make_mat4(GPU_GetProjection());
        glm::mat4 view = glm::make_mat4(GPU_GetView());
        GPU_SetActiveTarget(lightingTarget);
        GPU_EnableCamera(lightingTarget, false);
        GPU_SetCamera(lightingTarget, nullptr);
        GPU_ClearColor(lightingTarget, { 0, 0, 0, 0 });

        float xCenter = position.x + (intensity * Renderer::unitSize * 0.5f);
        float yCenter = position.y + (intensity * Renderer::unitSize * 0.5f);

        GPU_LoadIdentity();
        glm::mat4 model = glm::make_mat4(GPU_GetModel());
        model = glm::translate(model, { xCenter, yCenter, 0 });
        model = glm::translate(model, { position.x + lightingTexture->w, position.y + lightingTexture->h, position.z });
        model = glm::scale(model, { 1, 1, intensity * Renderer::unitSize });
        model = glm::translate(model, { -xCenter, -yCenter, 0 });
        GPU_SetModel(glm::value_ptr(model));
        GPU_SetView(glm::value_ptr(view));
        GPU_SetProjection(glm::value_ptr(proj));
        //glm::mat4 mv = model * view;

        //glm::vec4 vector = glm::vec4(position, 0.0f) * mv;

        glm::mat4 mvp = proj * view * model;

        GPU_ShaderBlock block = GPU_LoadShaderBlock(shaderObj, "a_Position", "a_Texcoord", "v_vColour", nullptr);

        float lightPos[3];
        lightPos[0] = position.x + lightingTexture->w * 0.5f + Renderer::unitSize * 0.5f;
        lightPos[1] = -position.y + lightingTexture->h * 0.5f;
        lightPos[2] = position.z;
        float lightColor[4];
        lightColor[0] = 255;
        lightColor[1] = 255;
        lightColor[2] = 255;
        lightColor[3] = 255;
        GPU_ActivateShaderProgram(shaderObj, &block);
        //GPU_SetUniformMatrixfv(GPU_GetUniformLocation(shaderObj, "u_MVPMatrix"), 1, 4, 4, false, glm::value_ptr(mvp));
        //GPU_SetUniformfv(GPU_GetUniformLocation(shaderObj, "lightpos"), 3, 1, lightPos);
        glUniform3fv(GPU_GetUniformLocation(shaderObj, "lightpos"), 1, lightPos);
        GPU_SetUniformfv(GPU_GetUniformLocation(shaderObj, "lightColor"), 4, 1, lightColor);
        glUniformMatrix4fv(GPU_GetUniformLocation(shaderObj, "u_MVPMatrix"), 1, false, glm::value_ptr(mvp));
        //GPU_SetAttributefv(GPU_GetAttributeLocation(shaderObj, "a_Position"), 3, glm::value_ptr(position));
        //GPU_CircleFilled(lightingTarget, position.x, -position.y, intensity / 20.0f, { 255, 255, 255, 0 });
        glm::vec4 lightIntensity = { intensity, intensity, intensity, intensity };
        glUniform4fv(GPU_GetUniformLocation(shaderObj, "lightIntensity"), 1, glm::value_ptr(lightIntensity));

        lightingTexture->w = Renderer::viewportTarget->w;
        lightingTexture->h = Renderer::viewportTarget->h;
        GPU_SetImageVirtualResolution(lightingTexture, lightingTexture->w, lightingTexture->h);
        GPU_SetBlending(lightingTexture, true);
        GPU_SetBlendMode(lightingTexture, GPU_BLEND_ADD);
        GPU_SetImageFilter(lightingTexture, GPU_FILTER_LINEAR_MIPMAP);
        GPU_Rect rect;
        rect.x = -position.x;
        rect.y = -position.y;
        rect.w = lightingTexture->w;
        rect.h = lightingTexture->h;
        GPU_Flip(lightingTarget);
        //GPU_BlitRect(lightingTexture, nullptr, Renderer::viewportTarget, nullptr);
        GPU_DeactivateShaderProgram();
        GPU_SetActiveTarget(Renderer::viewportTarget);
        GPU_ShaderBlock newBlock = GPU_LoadShaderBlock(shaderObj, "a_Position", "a_Texcoord", "v_vColour", "u_MVPMatrix");
        GPU_ActivateShaderProgram(shaderObj, &newBlock);
        GPU_Blit(lightingTexture, nullptr, Renderer::viewportTarget, lightingTexture->w * 0.5f, lightingTexture->h * 0.5f);
        GPU_DeactivateShaderProgram();
    }
}