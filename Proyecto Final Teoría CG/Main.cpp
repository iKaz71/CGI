/*---------------------------------------------------------*/
/* ----------------   Proyecto --------------------------*/
/*-----------------    2020-1   ---------------------------*/
/*------------- Alumnos:
	Pérez Valdez Edgar Michael
	Santos Escobar Christian Alexis
---------------*/
//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>

#include "camera.h"
#include "Model.h"


// Other Libs
#include "SOIL2/SOIL2.h"


void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(-6.0f, 3.0f, -3.0f));
double	lastX = 0.0f,
lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 2.0f, 2.0f);
glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);

void myData(void);
void display(Shader, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

bool puerta = false;
float rotP = 0.0f;

//Texture
unsigned int	t_smile;

//Keyframes
float posY = 3.5f, posX, posZ;

float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f;

float movLuzX = 0.0f;

#define MAX_FRAMES 5
int i_max_steps = 15;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 5;			//introducir datos
bool play = false;
int playIndex = 5;

void saveFrame(void)
{

	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = 3.5f;
	KeyFrame[0].posZ = 0.0f;

	KeyFrame[1].posX = 0.0f;
	KeyFrame[1].posY = 3.0f;
	KeyFrame[1].posZ = 0.0f;

	KeyFrame[2].posX = 0.0f;
	KeyFrame[2].posY = 2.0f;
	KeyFrame[2].posZ = 0.0f;

	KeyFrame[3].posX = 0.0f;
	KeyFrame[3].posY = 1.0f;
	KeyFrame[3].posZ = 0.0f;

	KeyFrame[4].posX = 0.0f;
	KeyFrame[4].posY = 0.5f;
	KeyFrame[4].posZ = 0.0f;

	FrameIndex = 5;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;


}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

}


unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{


}

void myData()
{
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			i_curr_steps++;
		}

	}

}

void display(Shader shader, Model casa, Model piso, Model mesa, Model computadora, Model computadoraInv, Model muro1,
	Model muro2, Model muro3, Model muro4, Model calavera, Model techo, Model slender, Model space, Model pasto, Model door,
	Model mano, Model ofrenda, Model bat)
{
	shader.use();

	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	
	shader.setVec3("dirLight.ambient", glm::vec3(0.6f, 0.6f, 0.6f));
	shader.setVec3("dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
	
	//shader.setVec3("pointLight.position", lightPosition);
	shader.setVec3("pointLight.position", glm::vec3(0.0f, 8.0f, 0.0f));
	
	shader.setVec3("pointLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
	shader.setVec3("pointLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
	shader.setVec3("pointLight.specular", glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setFloat("pointLight.constant", 0.0f);
	shader.setFloat("pointLight.linear", 0.09f);
	shader.setFloat("pointLight.quadratic", 0.032f);
	

	shader.setFloat("material_shininess", 32.0f);
	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -0.85f));
	model = glm::scale(model, glm::vec3(0.12f, 0.06f, 0.12f));
	shader.setMat4("model", model);
	piso.Draw(shader);

	//Mesa1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, -4.0f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.055f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Computadora 1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, -6.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);
	//Computadora 2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, -4.4f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);
	//Computadora 3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, -2.5f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);

	//Mesa2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 4.4f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.063f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Computadora 4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 1.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 5
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 2.7f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 6
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 4.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 7
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 5.9f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);



	//Mesa3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.055f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Computadora 8
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);
	//Computadora 9
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.4f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);
	//Computadora 10
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);



	//Mesa4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 4.4f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.063f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Computadora 11
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 12
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.7f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 13
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 4.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 14
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.9f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);



	//Mesa5
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, -4.0f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.055f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Computadora 15
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, -6.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);
	//Computadora 16
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, -4.4f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);
	//Computadora 17
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, -2.5f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadoraInv.Draw(shader);




	//Mesa6
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 4.4f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.063f));
	shader.setMat4("model", model);
	mesa.Draw(shader);
	//Computadora 18
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 1.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 19
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 2.7f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 20
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 4.3f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);
	//Computadora 21
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 5.9f));
	model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
	shader.setMat4("model", model);
	computadora.Draw(shader);

	//Muro1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.3f, 0.0f, -7.0f));
	model = glm::scale(model, glm::vec3(0.085f, 0.045f, 0.03f));
	shader.setMat4("model", model);
	muro1.Draw(shader);

	//Muro2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 0.0f, 8.0f));
	model = glm::scale(model, glm::vec3(0.097f, 0.045f, 0.03f));
	shader.setMat4("model", model);
	muro2.Draw(shader);

	//Muro3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-8.8f, 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(0.083f, 0.045f, 0.082f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	muro3.Draw(shader);

	//Muro4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(8.8f, 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(0.083f, 0.045f, 0.082f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	muro4.Draw(shader);

	//Slenderman
	model = glm::translate(glm::mat4(1.0f), glm::vec3(18.0f, 8.0f, 16.0f));
	model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
	model = glm::rotate(model, glm::radians(-130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	slender.Draw(shader);


	//Kunai
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, posY, 0.0f));
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
	shader.setMat4("model", model);
	calavera.Draw(shader);

	//Techo
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -0.2f, -0.85f));
	model = glm::scale(model, glm::vec3(0.12f, 0.06f, 0.12f));
	shader.setMat4("model", model);
	techo.Draw(shader);

	//CASA
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 0.0f, -24.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	shader.setMat4("model", model);
	casa.Draw(shader);

	//Puerta
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.1f, 0.0f, -24.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(rotP), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	door.Draw(shader);

	//Noche
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	space.Draw(shader);

	//Pasto
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.1f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	pasto.Draw(shader);

	//Mano
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.78f, 4.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	mano.Draw(shader);

	//Mano1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -1.78f, -4.4f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	mano.Draw(shader);

	//Ofrenda
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	ofrenda.Draw(shader);

	//bAT
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	bat.Draw(shader);


	
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	
	glfwInit();










	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, my_input);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	// Load models
	Model pisoModel = ((char *)"Modelos/Suelo/suelo.obj");
	Model mesa = ((char *)"Modelos/Mesa/mesa.obj");
	Model computadora = ((char *)"Modelos/PC/computadora.obj");
	Model computadoraInv = ((char *)"Modelos/PC/computadoraInv.obj");
	Model muro1 = ((char *)"Modelos/Ventana/muro1.obj");
	Model muro2 = ((char *)"Modelos/Ventana/muro2.obj");
	Model muro3 = ((char *)"Modelos/Ventana/muro3.obj");
	Model muro4 = ((char *)"Modelos/Ventana/muro4.obj");
	Model cala = ((char *)"Modelos/Kunai/kunai.obj");
	Model techo = ((char *)"Modelos/Tejado/techo.obj");
	Model casa = ((char *)"Modelos/Casa/micasanavi.obj");
	Model slender = ((char *)"Modelos/Slenderman/slender.obj");
	Model space = ((char *)"Modelos/Noche/noche.obj");
	Model pasto = ((char *)"Modelos/Grass/grass.obj");
	Model puerta = ((char *)"Modelos/Puerta/puerta.obj");
	Model mano = ((char *)"Modelos/Mano/RiggedHand.obj");
	Model ofrenda = ((char *)"Modelos/Ofrenda/ofrenda.obj");
	Model bat = ((char *)"Modelos/Bat/BatBody/bat.obj");
	
	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
	}

	saveFrame();

	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame+ 0.01;
		lastFrame = currentFrame;
		// input
		// -----
		//my_input(window);
		animate();

		// render
		// Backgound color
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//display(modelShader, ourModel, llantasModel);
		display(modelShader, casa, pisoModel, mesa, computadora, computadoraInv, muro1, muro2, muro3, muro4, cala, techo, slender, space, pasto, puerta, mano, ofrenda, bat);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		movLuzX++;
	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
		movLuzX--;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{	
		if (puerta == false)
		{
			for (int i = 0; i < 20; i++)
			{
				rotP -= 1.0f;
			}
			puerta = true;
		}
		else
		{
			for (int i = 0; i < 20; i++)
			{
				rotP += 1.0f;
			}
			puerta = false;
		}
	}


	//To play KeyFrame animation 
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
	}

	//To Save a KeyFrame	
	/*
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
	*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}