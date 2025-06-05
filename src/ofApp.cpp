// src/ofApp.cpp
#include "ofApp.h"

// HSV → RGB helper
static ofColor hsv2rgb(float h, float s, float v)
{
    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6)
    {
    case 0:
        return ofColor(v * 255, t * 255, p * 255);
    case 1:
        return ofColor(q * 255, v * 255, p * 255);
    case 2:
        return ofColor(p * 255, v * 255, t * 255);
    case 3:
        return ofColor(p * 255, q * 255, v * 255);
    case 4:
        return ofColor(t * 255, p * 255, v * 255);
    case 5:
        return ofColor(v * 255, p * 255, q * 255);
    }
    return ofColor(0);
}

void ofApp::setup()
{
    ofSetFrameRate(60);

    // init Kinect: RGB + depth
    kinect.init(false, true);
    kinect.open();
    tiltAngle = kinect.getCurrentCameraTiltAngle();

    // allocate buffers
    rawDepth.allocate(640, 480, OF_PIXELS_MONO);
    tmpDepth.allocate(640, 480, OF_PIXELS_MONO);
    maskPixels.allocate(640, 480, OF_PIXELS_GRAY);
    maskTex.allocate(640, 480, GL_LUMINANCE);
    colorPixels.allocate(640, 480, OF_PIXELS_RGB);
    colorTex.allocate(640, 480, GL_RGB);

    // name Syphon servers
    rgbSrv.setName("Kinect RGB");
    maskSrv.setName("Kinect DepthMask");
    colorSrv.setName("Kinect DepthColor");
}

void ofApp::update()
{
    kinect.update();
    if (!kinect.isFrameNew())
        return;

    // 1) publish RGB
    rgbSrv.publishTexture(&kinect.getTexture());

    // grab raw depth
    rawDepth = kinect.getRawDepthPixels();
    tmpDepth = rawDepth; // copy for median

    int w = rawDepth.getWidth();
    int h = rawDepth.getHeight();

    // 2) 3×3 median filter on rawDepth
    for (int y = 1; y < h - 1; y++)
    {
        for (int x = 1; x < w - 1; x++)
        {
            uint16_t vals[9];
            int k = 0;
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    vals[k++] = tmpDepth.getPixels()[(y + dy) * w + (x + dx)];
                }
            }
            std::nth_element(vals, vals + 4, vals + 9);
            rawDepth.getPixels()[y * w + x] = vals[4];
        }
    }

    // 3) build & publish binary mask
    for (int i = 0; i < w * h; i++)
    {
        uint16_t d = rawDepth.getPixels()[i];
        maskPixels.getPixels()[i] = (d >= minDepth && d <= maxDepth) ? 255 : 0;
    }
    maskTex.loadData(maskPixels);
    maskSrv.publishTexture(&maskTex);

    // 4) build & publish false‑color depth
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            uint16_t d = rawDepth.getPixels()[y * w + x];
            float norm = ofClamp((d - minDepth) / (maxDepth - minDepth), 0.0f, 1.0f);
            // map [0..1]→ hue [0.6..0] (blue→red)
            ofColor col = hsv2rgb((1.0f - norm) * 0.6f, 1.0f, 1.0f);
            colorPixels.setColor(x, y, col);
        }
    }
    colorTex.loadData(colorPixels);
    colorSrv.publishTexture(&colorTex);
}

void ofApp::draw()
{
    ofBackground(0);

    // draw three 320×240 panes
    kinect.getTexture().draw(0, 0, 320, 240);
    maskTex.draw(320, 0, 320, 240);
    colorTex.draw(640, 0, 320, 240);

    // HUD
    std::stringstream ss;
    ss << "Tilt: ↑/↓ → " << tiltAngle << "°\n\n";
    ss << "A/S minDepth: " << minDepth << " mm\n";
    ss << "Z/X maxDepth: " << maxDepth << " mm";
    ofDrawBitmapStringHighlight(ss.str(), 10, 260);
}

void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case OF_KEY_UP:
        tiltAngle = ofClamp(tiltAngle + 5, -30, 30);
        kinect.setCameraTiltAngle(tiltAngle);
        break;
    case OF_KEY_DOWN:
        tiltAngle = ofClamp(tiltAngle - 5, -30, 30);
        kinect.setCameraTiltAngle(tiltAngle);
        break;
    case 'a':
        minDepth = ofClamp(minDepth + 50, 0, maxDepth);
        break;
    case 's':
        minDepth = ofClamp(minDepth - 50, 0, maxDepth);
        break;
    case 'z':
        maxDepth = ofClamp(maxDepth + 50, minDepth, 2047);
        break;
    case 'x':
        maxDepth = ofClamp(maxDepth - 50, minDepth, 2047);
        break;
    }
}

void ofApp::exit()
{
    kinect.close();
}
