#pragma once

#include <vector>
#include <cmath>
#include <glew.h> 
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

struct TrackPoint {
    glm::vec3 position;
    float bankingAngle;
    bool isJump;
};

class PlanetaryRaceTrack {
public:
    GLuint vaoID = 0, vboID = 0, eboID = 0;
    GLsizei totalIndices = 0;

    std::vector<TrackPoint> controlPoints;
    int smoothSteps = 25;
    float width = 10.0f;
    float wallH = 0.8f;
    float wallW = 0.6f;

    struct PathSample {
        glm::vec3 pos;
        glm::vec3 fwd;
        glm::vec3 up;
        glm::vec3 right;
    };
    std::vector<PathSample> pathData;

    PlanetaryRaceTrack() {
        setupDefaultTrack();
    }

    ~PlanetaryRaceTrack() {
        if (eboID) glDeleteBuffers(1, &eboID);
        if (vboID) glDeleteBuffers(1, &vboID);
        if (vaoID) glDeleteVertexArrays(1, &vaoID);
    }

    void setupDefaultTrack() {
        controlPoints = {
            { glm::vec3(0.0f,  0.3f,    0.0f),   0.0f, false },
            { glm::vec3(55.0f,  0.3f,    0.0f),   0.0f, false },
            { glm::vec3(95.0f,  4.5f,   30.0f),  12.0f, false },
            { glm::vec3(105.0f, 14.0f,   85.0f),   8.0f, false },
            { glm::vec3(65.0f, 18.0f,  130.0f),  15.0f, false },
            { glm::vec3(15.0f, 23.0f,  135.0f),   0.0f, true },
            { glm::vec3(-35.0f,  8.0f,  125.0f),   0.0f, false },
            { glm::vec3(-80.0f,  2.5f,   95.0f), -12.0f, false },
            { glm::vec3(-55.0f,  1.5f,   55.0f),  12.0f, false },
            { glm::vec3(-85.0f,  0.5f,   15.0f), -15.0f, false },
            { glm::vec3(-45.0f,  0.3f,   -5.0f),   0.0f, false }
        };
    }

    static glm::vec3 interpolatePos(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) {
        float tSq = t * t;
        float tCub = tSq * t;
        return 0.5f * ((2.0f * p1) + (-p0 + p2) * t + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * tSq + (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * tCub);
    }

    static glm::vec3 interpolateDir(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) {
        float tSq = t * t;
        return glm::normalize(0.5f * ((-p0 + p2) + 2.0f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t + 3.0f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * tSq));
    }

    void buildMesh() {
        std::vector<float> vData;
        std::vector<unsigned int> iData;
        pathData.clear();

        int nPts = static_cast<int>(controlPoints.size());
        int totalFrames = nPts * smoothSteps;
        float uvMult = 0.20f;
        const int vertsPerSlice = 6;

        for (int i = 0; i <= totalFrames; ++i) {
            int segment = (i / smoothSteps) % nPts;
            float t = static_cast<float>(i % smoothSteps) / smoothSteps;
            if (i == totalFrames) { segment = 0; t = 0.0f; }

            const TrackPoint& n0 = controlPoints[(segment - 1 + nPts) % nPts];
            const TrackPoint& n1 = controlPoints[segment];
            const TrackPoint& n2 = controlPoints[(segment + 1) % nPts];
            const TrackPoint& n3 = controlPoints[(segment + 2) % nPts];

            glm::vec3 cPos = interpolatePos(n0.position, n1.position, n2.position, n3.position, t);
            glm::vec3 cDir = interpolateDir(n0.position, n1.position, n2.position, n3.position, t);

            glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
            glm::vec3 cRight = glm::normalize(glm::cross(cDir, globalUp));
            glm::vec3 cNorm = glm::normalize(glm::cross(cRight, cDir));

            float bAngle = glm::radians(glm::mix(n1.bankingAngle, n2.bankingAngle, t));
            glm::mat4 bRot = glm::rotate(glm::mat4(1.0f), bAngle, cDir);
            cRight = glm::normalize(glm::vec3(bRot * glm::vec4(cRight, 0.0f)));
            cNorm = glm::normalize(glm::vec3(bRot * glm::vec4(cNorm, 0.0f)));

            if (i < totalFrames) pathData.push_back({ cPos, cDir, cNorm, cRight });

            float hw = width * 0.5f;
            float vc = static_cast<float>(i) * uvMult;

            glm::vec3 pRL = cPos - cRight * hw;
            glm::vec3 pRR = cPos + cRight * hw;
            glm::vec3 pWL = pRL - cRight * wallW + cNorm * wallH;
            glm::vec3 pWR = pRR + cRight * wallW + cNorm * wallH;
            glm::vec3 pBL = pRL - cRight * wallW - cNorm * 0.8f;
            glm::vec3 pBR = pRR + cRight * wallW - cNorm * 0.8f;

            glm::vec3 pts[vertsPerSlice] = { pBL, pWL, pRL, pRR, pWR, pBR };
            float uvs[vertsPerSlice] = { 0.0f, 0.1f, 0.2f, 0.8f, 0.9f, 1.0f };
            glm::vec3 nms[vertsPerSlice] = { -cRight, cNorm, cNorm, cNorm, cNorm, cRight };

            for (int k = 0; k < vertsPerSlice; ++k) {
                vData.push_back(pts[k].x); vData.push_back(pts[k].y); vData.push_back(pts[k].z);
                vData.push_back(uvs[k]);   vData.push_back(vc);
                vData.push_back(nms[k].x); vData.push_back(nms[k].y); vData.push_back(nms[k].z);
            }
        }

        for (int i = 0; i < totalFrames; ++i) {
            int segment = (i / smoothSteps) % nPts;
            int localI = i % smoothSteps;

            if (controlPoints[segment].isJump && localI > (smoothSteps * 0.65f)) continue;

            unsigned int bCurr = i * vertsPerSlice;
            unsigned int bNext = (i + 1) * vertsPerSlice;

            for (int q = 0; q < vertsPerSlice - 1; ++q) {
                iData.push_back(bCurr + q); iData.push_back(bCurr + q + 1); iData.push_back(bNext + q);
                iData.push_back(bCurr + q + 1); iData.push_back(bNext + q + 1); iData.push_back(bNext + q);
            }
        }

        totalIndices = static_cast<GLsizei>(iData.size());

        glGenVertexArrays(1, &vaoID);
        glGenBuffers(1, &vboID);
        glGenBuffers(1, &eboID);

        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, vData.size() * sizeof(float), vData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, iData.size() * sizeof(unsigned int), iData.data(), GL_STATIC_DRAW);

        GLsizei st = 8 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, st, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, st, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, st, (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    void draw() const {
        glBindVertexArray(vaoID);
        glDrawElements(GL_TRIANGLES, totalIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};