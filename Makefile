CXX = g++

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl2-config --cflags` -g -W -Wall -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

LDFLAGS = `sdl2-config --libs` -lm -lexpat -lSDL2_ttf -lSDL2_image

OBJS = \
  renderContext.o \
	ioMod.o \
	parseXML.o \
	gamedata.o \
	viewport.o \
	world.o \
	unpack.o \
	frame.o \
	frameFactory.o \
	frameGenerator.o \
	sprite.o \
	multisprite.o \
	vector2f.o \
	audio.o \
	Queue.o \
	clock.o \
	engine.o \
	twoWaySprite.o \
	hud.o\
	player.o \
	smartSprite.o \
	subjectSprite.o \
	collisionStrategy.o \
	bullet.o \
	bulletPool.o \
	chunk.o \
	explodingSprite.o \
	main.o
EXEC = run

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
	rm -rf frames/*.bmp
