Evan Sinasac - 1081418
INFO6023 Algorithms and GEMS Midterm

Built and run in Visual Studios 2019 community in both Debug and Release x64, Release is much faster, I do recommend.  (I thought about recording a super quick demo but it's too late, I'm too tired and I just wanna submit it at this point)

CONTROLS (This is built off of the Graphics 2 Project 1 code I submitted, for sake of time I did not clean up the controls)
WASD			- Move camera
QE			- Raise/lower camera
Space			- Set view position (meant to be like a security camera or something)
B			- Turn debug objects on
M			- Change all models to wireframe
Keypad 1-6		- Change FBO resolution
P			- Set FBO resolution to current window size (important for when resizing window)
1-9			- Different Offscreen 2D Effects
0			- Clear Offscreen 2D Effects

Question 1:
	I wasn't fully certain, even after the clarification, what was and wasn't allowed for using STL containers.  What I settled on was anything we had before or were given it was ok, and anything else I needed to make for this program I used our custom containers... by which I mean the Smart Array because I'm a huge sucker for arrays and vectors.  So I used the templated Smart Array that I modified for the INFO6023 Project 1 that contains the removeAtIndex.  Otherwise I just took the indexes of the maze array and used those for the model locations in the world (so if the maze at 0,0 is a wall then the model at 0,0 was given a green colour).  I also copied the vector values into a Smart Array of the same format (basically the double array of bools).

Question 2:
	Took a random value, if it wasn't a "grass" portion of the maze then I repeated the loop until I made 10 plants on random portions of the maze.  I used one of the tree models from the Graphics 1 Final.  Then I made a cPlantEntity which causes them to grow by increasing the y value of the scale, and after the determined time it becomes "matured", which changes it's colour and causes it to stop growing.  The entire SA of plants is handled on one thread.

Question 3:
	To handle the herbivores (herbis) and carnivores (carnis) I took the same approach as with the plants in order to spawn them on the correct starting type of tile.  I then took them together as a set of entities (realized for the carnis eating I should've kept them separate, but at that point I was several hours in and giving up), making a SA of "cAnimals" which inherits from "Entity" and then made the "cHerbivore" and "cCarnivore", both of which inherit from "cAnimals".  This was mostly so I could do the threading a bit easier, going through the single list of animals and running their process in an individual thread each.  Also, I ended up keeping a copy of the maze on each Animal, so they can check their positions on the fly.  What this means is that as each move around they're actively checking to remain in bounds of the maze, and for the herbis, always on a grass square.  Both wander about, the herbis need a LOT of work, and the carnis work pretty well.  Carnis also check where they are every time they move and have a 25% percent chance of falling asleep while on a grass tile.
Carnis are represented by cubes that are pink when wandering and black when sleeping.  Herbis are represented by blue spheres.
Note: for both the plants and the animals, I tried implementing the drawing process as part of the threading, but they would not end up being drawn on the screen, so I basically ended up using the threading to handle the Process calls on each entity and then went through each list and drew their mesh after the threading was done.

Question 4:
	This is about where my sanity began to leave me.  You'll notice that the herbivores are kinda dumb, and by that I mean really dumb and I had little to no patience today, so I ended up just slapping some checks together.  What this ended up meaning is that the herbivores basically either don't move or gyrate between two spots... Yeah, idk.  I got a bit of sanity back after working on the carnis and they work much better, but then I had to come back to the herbis for their hunger cycle.  Welp, I tried a few different things, but they're still very, very dumb and usually end up in a corner.  In, idk, a lot of runs, I saw a herbi actually get to a plant once.  And I was testing something else so I didn't even see if it caused the plant to reset or not cause I closed the program as I saw the herbi approaching a plant.  That being said, the logic is there?  It *should work, if it ever gets to a plant.  The carnis, once again since they have a lot fewer checks, were much easier.  However, at this point, I really did not want to handle the "eating" portion of the SA, so it just prints out a message to the console when it eats a herbi.  The herbis also print a message just to know if they enter the if statement.  But yeah, I guess because it's not actually being eaten I'm not handling the second part where other carnis can't eat the same herbi.

Question 5:
	And I also did not get around to implementing the herbi detection of carnis.  Honestly, the amount of trouble I had with the herbis today was mind boggling, I should not be having this many problems.  But since they end up frozen in some place anyways, they kind of end up cornered whenever a carni startes hunting lol.  

Question 6:
	I was actually kind of excited to handle Question 6 when I read through the document, but after everything else, yeah no.  Did not end up doing Question 6 at all.

FOREWARNING:
	I've experienced a few memory leaks/location out of bounds crashes while running the program.  A lot of them were me being dumb and have been fixed.  HOWEVER, towards the end of this when I'm about to be submitting, the program still occasionnally crashes.  This has happened even before the visual window has opened.  I've attempted to track the memory leak, see where I'm trying to call an index that could possibly be out of bounds, but I can't find anything else to fix.  I've noticed that sometimes cleaning/re-building the solution fixes it, and the crashes are sporadic and, as far as I can tell, random.  If this happens to you, sincere apologies.  Please attempt a clean and/or re-build and run the program again.  
Side-note, my PC has a tool that allows me to see and clean up the available RAM, so I checked it a few times.  It's dipped below 2GB, which I haven't seen happen since I tried to run Cyberpunk.  When nothing is running there's usually 10GB free and when I have a few programs running and/or games I average 5GB, so idk why Visual Studios is taking up so much today.  I'm going to reset my PC after submitting.  Could be related issue, but idk.  (I'm just praying it never crashes for you lol).

Uhm... I think that's about it.  Models are the ones we've used in class, I ended up using the Invader Cube from the Game Engine final, a tree from the Graphics 1 final, and then just some of the other imposter shapes and things from class.  Shaders are the ones from class that I've slightly modified for the Graphics 2 Project 1, essentially to use the Dark Fantasy models (none of which I'm using in this project), which is why the controls for the different shader 2D effects are still around.  

Thank you, have a good day!

P.S.
I made a quick second attempt at the herbis' wander after writing this... and it did nothing, almost looks worse honestly, ugh lol.  I'm done, enjoy lol.