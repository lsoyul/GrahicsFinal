#include "Patternloader.h"


void clear()
{
	if (c == 30)
	{
		//next pattern create
		speed = rand() % 100 / 10000.0f;
		rotat1 = vec3(rand() % 10 / 10.0f, rand() % 10 / 10.0f, rand() % 10 / 10.0f);
		glClearColor((rand() % 50) / 100.0f, (rand() % 50) / 100.0f, (rand() % 50) / 100.0f, 1.0f);
		camera.eye = vec3(box[pat[curStage].start].position.x + 100.0f, 200, box[pat[curStage].start].position.z + 100.0f);
		up = true;
	}
}

void setcolors()
{
	if (isStart==1) colors = vec4(0.8f, 1.0f, 0.0f, 0.0f);
	if (rand() % 2 == 1)
		colors.x += (rand() % 2) / 10.0f;
	else
		colors.x -= (rand() % 2) / 10.0f;
	if (rand() % 2 == 1)
		colors.y += (rand() % 2) / 10.0f;
	else
		colors.y -= (rand() % 2) / 10.0f;
	if (rand() % 2 == 1)
		colors.z += (rand() % 2) / 10.0f;
	else
		colors.z -= (rand() % 2) / 10.0f;

	if (colors.x > 1.0f)
		colors.x = 1.0f;
	if (colors.y > 1.0f)
		colors.y = 1.0f;
	if (colors.z > 1.0f)
		colors.z = 1.0f;

	if (colors.x < 0.0f)
		colors.x = 0.0f;
	if (colors.y < 0.0f)
		colors.y = 0.0f;
	if (colors.z < 0.0f)
		colors.z = 0.0f;

}

void setHeight()
{
	float x = (60.0f / (float)c) - 2;
	x = x / 29.0f;
	screenheight = x;
}


void boom(int i)
{
	if (box[i].boom == false)
	{
		if (boombit == false)
		{
			printf("BOOM\n");
			boombit = true;
			if (game == true)
			{	
				g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[14], 0, &g_Channel[2]);
				boombit = false;
			}
		}
		box[i].boom = true;
	}
	else
	{
		boombit = false;
		box[i].boom = false;
	}

	//play boom animation
}

void afterboom(int i)
{
	box[i].boom = false;

	//stop boom animation
}

void hit()
{
	int i = (int)camera.eye.z / 200;
	int j = (int)camera.eye.x / 200;
	if (box[10 * i + j].boom == true)
	{
		//printf("BOOMMMMMMMMMMMMMMMM!!!!!!\n");
		if (up == true)
		{
			game = false;
			g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[11], 0, &g_Channel[4]);

			screenbit = 1;
		}
	}
}

void finish()
{
	int i = (int)camera.eye.z / 200;
	int j = (int)camera.eye.x / 200;
	
	if (10 * i + j == pat[curStage].finish)
	{
		if (up == true)
		{
			printf("finish!\n");
			game = false;
			g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[15], 0, &g_Channel[3]);
			if (currentbgm < 9)
			{
				currentbgm++;
				g_Channel[0]->stop();
				g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[currentbgm], 0, &g_Channel[0]);
			}
			screenbit = 1;

			for (int k = 0; k < 100; k++){
				box[k].visible = false;
				spikes[k].visible = false;
			}

			switch (curStage)
			{
			case 0:
				curStage = 1; break;
			case 1:
				curStage = 2; break;
			case 2:
				curStage = 3; break;
			case 3:
				curStage = 4; break;
			case 4:
				curStage = 5; break;
			case 5:
				curStage = 6; break;
			case 6:
				curStage = 7; break;
			case 7:
				curStage = 8; break;
			case 8:
				curStage = 9; break;
			case 9:
				curStage = 0; break;
			default:
				break;
			}
			/*for (uint i = 0; i < pat[curStage].visibles.size(); i++){
				box[pat[curStage].visibles[i]].visible = true;
				spikes[pat[curStage].visibles[i]].visible = true;
				}*/
		}
	}
}

void falling()
{
	if (camera.eye.y < -3000)
	{
		game = false;
		g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[13], 0, &g_Channel[4]);

		screenbit = 1;
	}
}

void bloomfoottile()
{
	
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (camera.eye.x > j * 200.0f && camera.eye.x < (j + 1) * 200.0f)
			{
				if (camera.eye.z > i * 200.0f && camera.eye.z < (i + 1) * 200.0f)
				{
					box[10 * i + j].bloom = true;
				}
				else
				{
					box[10 * i + j].bloom = false;
				}
			}
			else
			{
				box[10 * i + j].bloom = false;
			}
		}
	}
}
