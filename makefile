CXX=g++
INCLUDES=-Irtaudio/ -Istk/ -Ix-api/ -Iy-api -I/opt/local/include -I/usr/local/include 
FLAGS=-D__MACOSX_CORE__ $(INCLUDES) -c
LIBS=-framework CoreAudio -framework CoreMIDI -framework CoreFoundation \
        -framework IOKit -framework Carbon -framework OpenGL \
        -framework GLUT -lstdc++ -lm -L/usr/local/lib -lfluidsynth -lsndfile -lpng 
OBJS=Home.o core/jgh-audio.o core/jgh-entity.o core/jgh-sim.o \
	core/jgh-gfx.o core/jgh-sndsrc.o core/jgh-globals.o core/glm.o \
	core/jgh-person.o core/jgh-pe.o core/jgh-localities.o core/jgh-house.o \
	x-api/x-audio.o x-api/x-buffer.o x-api/x-fun.o x-api/x-gfx.o \
	x-api/x-loadlum.o x-api/x-loadrgb.o x-api/x-thread.o x-api/x-vector3d.o \
	y-api/y-charting.o y-api/y-echo.o y-api/y-entity.o y-api/y-fft.o \
	y-api/y-fluidsynth.o y-api/y-particle.o y-api/y-score-reader.o y-api/y-waveform.o \
	rtaudio/RtAudio.o stk/Delay.o stk/DelayL.o stk/MidiFileIn.o \
	stk/Stk.o stk/NRev.o 

Home: $(OBJS)
	$(CXX) -o Home $(OBJS) $(LIBS)

Home.o: Home.cpp
	$(CXX) -o Home.o $(FLAGS) Home.cpp

core/jgh-audio.o: core/jgh-audio.h core/jgh-audio.cpp
	$(CXX) -o core/jgh-audio.o $(FLAGS) core/jgh-audio.cpp

core/jgh-entity.o: core/jgh-entity.h core/jgh-entity.cpp
	$(CXX) -o core/jgh-entity.o $(FLAGS) core/jgh-entity.cpp

core/jgh-sim.o: core/jgh-sim.h core/jgh-sim.cpp
	$(CXX) -o core/jgh-sim.o $(FLAGS) core/jgh-sim.cpp

core/jgh-gfx.o: core/jgh-gfx.h core/jgh-gfx.cpp
	$(CXX) -o core/jgh-gfx.o $(FLAGS) core/jgh-gfx.cpp

core/jgh-sndsrc.o: core/jgh-sndsrc.h core/jgh-sndsrc.cpp
	$(CXX) -o core/jgh-sndsrc.o $(FLAGS) core/jgh-sndsrc.cpp

core/jgh-globals.o: core/jgh-globals.h core/jgh-globals.cpp
	$(CXX) -o core/jgh-globals.o $(FLAGS) core/jgh-globals.cpp

core/glm.o: core/glm.h core/glm.cpp
	$(CXX) -o core/glm.o $(FLAGS) core/glm.cpp

core/jgh-person.o: core/jgh-person.h core/jgh-person.cpp
	$(CXX) -o core/jgh-person.o $(FLAGS) core/jgh-person.cpp

core/jgh-pe.o: core/jgh-pe.h core/jgh-pe.cpp
	$(CXX) -o core/jgh-pe.o $(FLAGS) core/jgh-pe.cpp

core/jgh-localities.o: core/jgh-localities.h core/jgh-localities.cpp
	$(CXX) -o core/jgh-localities.o $(FLAGS) core/jgh-localities.cpp

core/jgh-house.o: core/jgh-house.h core/jgh-house.cpp
	$(CXX) -o core/jgh-house.o $(FLAGS) core/jgh-house.cpp

x-api/x-audio.o: x-api/x-audio.h x-api/x-audio.cpp
	$(CXX) -o x-api/x-audio.o $(FLAGS) x-api/x-audio.cpp

x-api/x-buffer.o: x-api/x-buffer.h x-api/x-buffer.cpp
	$(CXX) -o x-api/x-buffer.o $(FLAGS) x-api/x-buffer.cpp

x-api/x-fun.o: x-api/x-fun.h x-api/x-fun.cpp
	$(CXX) -o x-api/x-fun.o $(FLAGS) x-api/x-fun.cpp

x-api/x-gfx.o: x-api/x-gfx.h x-api/x-gfx.cpp
	$(CXX) -o x-api/x-gfx.o $(FLAGS) x-api/x-gfx.cpp

x-api/x-loadlum.o: x-api/x-loadlum.h x-api/x-loadlum.cpp
	$(CXX) -o x-api/x-loadlum.o $(FLAGS) x-api/x-loadlum.cpp

x-api/x-loadrgb.o: x-api/x-loadrgb.h x-api/x-loadrgb.cpp
	$(CXX) -o x-api/x-loadrgb.o $(FLAGS) x-api/x-loadrgb.cpp

x-api/x-thread.o: x-api/x-thread.h x-api/x-thread.cpp
	$(CXX) -o x-api/x-thread.o $(FLAGS) x-api/x-thread.cpp

x-api/x-vector3d.o: x-api/x-vector3d.h x-api/x-vector3d.cpp
	$(CXX) -o x-api/x-vector3d.o $(FLAGS) x-api/x-vector3d.cpp

y-api/y-charting.o: y-api/y-charting.h y-api/y-charting.cpp
	$(CXX) -o y-api/y-charting.o $(FLAGS) y-api/y-charting.cpp

y-api/y-echo.o: y-api/y-echo.h y-api/y-echo.cpp
	$(CXX) -o y-api/y-echo.o $(FLAGS) y-api/y-echo.cpp

y-api/y-entity.o: y-api/y-entity.h y-api/y-entity.cpp
	$(CXX) -o y-api/y-entity.o $(FLAGS) y-api/y-entity.cpp

y-api/y-fft.o: y-api/y-fft.h y-api/y-fft.cpp
	$(CXX) -o y-api/y-fft.o $(FLAGS) y-api/y-fft.cpp

y-api/y-fluidsynth.o: y-api/y-fluidsynth.h y-api/y-fluidsynth.cpp
	$(CXX) -o y-api/y-fluidsynth.o $(FLAGS) y-api/y-fluidsynth.cpp

y-api/y-particle.o: y-api/y-particle.h y-api/y-particle.cpp
	$(CXX) -o y-api/y-particle.o $(FLAGS) y-api/y-particle.cpp

y-api/y-score-reader.o: y-api/y-score-reader.h y-api/y-score-reader.cpp
	$(CXX) -o y-api/y-score-reader.o $(FLAGS) y-api/y-score-reader.cpp

y-api/y-waveform.o: y-api/y-waveform.h y-api/y-waveform.cpp
	$(CXX) -o y-api/y-waveform.o $(FLAGS) y-api/y-waveform.cpp

rtaudio/RtAudio.o: rtaudio/RtAudio.h rtaudio/RtAudio.cpp
	$(CXX) -o rtaudio/RtAudio.o $(FLAGS) rtaudio/RtAudio.cpp

stk/Delay.o: stk/Delay.h stk/Delay.cpp
	$(CXX) -o stk/Delay.o $(FLAGS) stk/Delay.cpp

stk/DelayL.o: stk/DelayL.h stk/DelayL.cpp
	$(CXX) -o stk/DelayL.o $(FLAGS) stk/DelayL.cpp

stk/MidiFileIn.o: stk/MidiFileIn.h stk/MidiFileIn.cpp
	$(CXX) -o stk/MidiFileIn.o $(FLAGS) stk/MidiFileIn.cpp

stk/Stk.o: stk/Stk.h stk/Stk.cpp
	$(CXX) -o stk/Stk.o $(FLAGS) stk/Stk.cpp

stk/NRev.o: stk/NRev.h stk/NRev.cpp
	$(CXX) -o stk/NRev.o $(FLAGS) stk/NRev.cpp

clean:
	rm -f *~ *# *.o */*.o Home
