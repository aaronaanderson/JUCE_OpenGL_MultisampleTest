#pragma once

#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class OpenGLRenderer : public juce::Component, 
                       private juce::OpenGLRenderer, 
                       private juce::Timer
{
public:
    OpenGLRenderer()
    {
#ifdef JUCE_MAC
        glContext.setOpenGLVersionRequired (juce::OpenGLContext::OpenGLVersion::openGL4_1);
#else
        glContext.setOpenGLVersionRequired (juce::OpenGLContext::OpenGLVersion::defaultGLVersion);
#endif
        glContext.setRenderer (this);       
    
        juce::OpenGLPixelFormat pf;
        pf.multisamplingLevel = 4;
        glContext.setPixelFormat (pf);
        glContext.setMultisamplingEnabled (true);
        glContext.setComponentPaintingEnabled(false);
        glContext.attachTo (*this);
        startTimerHz (60);
    }
    ~OpenGLRenderer() override { glContext.detach(); }
    void resized() override { bounds = getLocalBounds(); }
private:
    juce::OpenGLContext glContext;
    juce::CriticalSection mutex;
    juce::Rectangle<int> bounds;

    void timerCallback() override { glContext.triggerRepaint(); }
    void newOpenGLContextCreated() override {}
    void renderOpenGL() override 
    {
        const juce::ScopedLock lock (mutex);
        juce::OpenGLHelpers::clear (getLookAndFeel().findColour (juce::DocumentWindow::backgroundColourId));
        juce::gl::glClear (juce::gl::GL_COLOR_BUFFER_BIT | juce::gl::GL_DEPTH_BUFFER_BIT);
        auto desktopScale = static_cast<float> (glContext.getRenderingScale());
        juce::gl::glDepthFunc (juce::gl::GL_LESS);
        juce::gl::glEnable (juce::gl::GL_MULTISAMPLE);
        
        juce::gl::glViewport (0, 0, 
                              juce::roundToInt(desktopScale * static_cast<float>(bounds.getWidth())), 
                              juce::roundToInt(desktopScale * static_cast<float>(bounds.getHeight())));   
    }
    void openGLContextClosing() override {}
};
//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
private:
    AudioPluginAudioProcessor& processorRef;
    OpenGLRenderer renderer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
