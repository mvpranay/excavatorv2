#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "gl_framework.hpp"


namespace csX75	 { 

	// A simple class that represents a node in the hierarchy tree
	class HNode {
		//glm::vec4 * vertices;
		//glm::vec4 * colors;
		
		std::size_t vertex_buffer_size;
		std::size_t color_buffer_size;
		
		GLuint num_vertices;
		GLuint vao,vbo;
		
		// glm::mat4 rotation;
		// glm::mat4 translation;
		
		std::vector<HNode*> children;
		HNode* parent;
		
		public:
		void update_matrices();
		glm::vec3 scale;
		glm::vec3 pivotOffset;
		GLfloat tx,ty,tz,rx,ry,rz;
		// one model matrix for each object
		glm::mat4 modelMatrix;
		
		HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec4* a_colours, std::size_t v_size, std::size_t c_size,
		glm::vec3 __scale, glm::vec3 __pivotOffset);
		//HNode (HNode* , glm::vec4*,  glm::vec4*,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		
		void add_child(HNode*);
		// void render();
		void change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz);
		void render_tree(const glm::mat4 &);
		void inc_rx();
		void inc_ry();
		void inc_rz();
		void dec_rx();
		void dec_ry();
		void dec_rz();
	};

	glm::mat4* multiply_stack(std::vector <glm::mat4> );
};	

#endif