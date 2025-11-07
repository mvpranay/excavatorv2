#include "hierarchy_node.hpp"

#include <iostream>

using namespace std;

extern GLuint modelLoc;

vector<glm::mat4> matrixStack;

namespace csX75
{

	HNode::HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec4* a_colours, std::size_t v_size, 
		std::size_t c_size, glm::vec3 __scale, glm::vec3 __pivotOffset){

		num_vertices = num_v;
		vertex_buffer_size = v_size;
		color_buffer_size = c_size;

		scale = __scale;
		pivotOffset = __pivotOffset;
		// initialize vao and vbo of the object;

		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours );

		// Position attribute
		glEnableVertexAttribArray(0); // layout(location = 0) aPos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		// Normal attribute
		glEnableVertexAttribArray(1); // layout(location = 1) aNormal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		// TexCoord attribute
		glEnableVertexAttribArray(2); // layout(location = 2) aTexCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		// set parent

		if(a_parent == NULL){
			parent = NULL;
		}
		else{
			parent = a_parent;
			parent->add_child(this);
		}

		//initial parameters are set to 0;

		tx=ty=tz=rx=ry=rz=0;

		update_matrices();
	}

	void HNode::update_matrices() {
		glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
		glm::mat4 Rpivot = glm::translate(glm::mat4(1.0f), pivotOffset);
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1,0,0))
					* glm::rotate(glm::mat4(1.0f), glm::radians(ry), glm::vec3(0,1,0))
					* glm::rotate(glm::mat4(1.0f), glm::radians(rz), glm::vec3(0,0,1));
		glm::mat4 RpivotBack = glm::translate(glm::mat4(1.0f), -pivotOffset);

		modelMatrix = T * Rpivot * R * RpivotBack; // exclude scale here
	}

	void HNode::add_child(HNode* a_child){
		children.push_back(a_child);
	}

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;

		update_matrices();
	}

	void HNode::render_tree(const glm::mat4& parentMatrix) {
		// Combine parent matrix with this node's modelMatrix
		glm::mat4 globalMatrix = parentMatrix * modelMatrix;
		glm::mat4 scaledMatrix = glm::scale(globalMatrix, scale);
		// Send to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(scaledMatrix));

		// Draw this node
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		// Recursively render children
		for (HNode* child : children) {
			child->render_tree(globalMatrix);
		}
	}

	void HNode::inc_rx(){
		rx++;
		update_matrices();
	}


	void HNode::inc_ry(){
		ry++;
		update_matrices();
	}

	void HNode::inc_rz(){
		rz++;
		update_matrices();
	}

	void HNode::dec_rx(){
		rx--;
		update_matrices();
	}

	void HNode::dec_ry(){
		ry--;
		update_matrices();
	}

	void HNode::dec_rz(){
		rz--;
		update_matrices();
	}


	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
		int mat_size = matStack.size();
		for(int i=0;i<mat_size;i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

}