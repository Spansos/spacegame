#include "graphics/renderer.hpp"
#include "graphics/load_shader.hpp"

Renderer::Renderer
(
    std::string fragment_shader,
    std::string vertex_shader
) :
    _program_id( LoadShaders( vertex_shader.c_str(), fragment_shader.c_str() ) ),
    _mvp_id( glGetUniformLocation( _program_id, "MVP" ) )
{
    // vertex array
    glGenVertexArrays( 1, &_vertex_array_id );
    glBindVertexArray( _vertex_array_id );

    // generate buffers
    glGenBuffers( 1, &_position_buffer_id );
    glGenBuffers( 1, &_normal_buffer_id );
    glGenBuffers( 1, &_color_buffer_id );

    // set vertex attribs
    glBindBuffer(GL_ARRAY_BUFFER, _position_buffer_id);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    glBindBuffer(GL_ARRAY_BUFFER, _normal_buffer_id);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    glBindBuffer(GL_ARRAY_BUFFER, _color_buffer_id);
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    // set state back
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

Renderer::~Renderer() {
    // delete generated gl objects
    glDeleteVertexArrays( 1, &_vertex_array_id );
    glDeleteBuffers( 1, &_position_buffer_id );
    glDeleteBuffers( 1, &_normal_buffer_id );
    glDeleteBuffers( 1, &_color_buffer_id );
    glDeleteProgram( _program_id );
}

void Renderer::draw( Window & window, const Mesh & mesh, glm::mat4 model_mat, const Camera & camera ) {
    glfwMakeContextCurrent( window.raw() );

    // bind our vertex array
    glBindVertexArray( _vertex_array_id );

    // send data to buffers
    glBindBuffer(GL_ARRAY_BUFFER, _position_buffer_id );
    glBufferData(GL_ARRAY_BUFFER, mesh.positions.size() * sizeof(glm::vec3), mesh.positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, _normal_buffer_id );
    glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(glm::vec3), mesh.normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, _color_buffer_id );
    glBufferData(GL_ARRAY_BUFFER, mesh.colors.size() * sizeof(glm::vec3), mesh.colors.data(), GL_STATIC_DRAW);

    // use the shader
    glUseProgram( _program_id );

    // uniforms
    glUniformMatrix4fv( _mvp_id, 1, GL_FALSE, &camera.calcMvp(model_mat)[0][0] );

    // enable attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // draw settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LESS);
    glEnable(GL_CULL_FACE);

    // draw
    glDrawArrays( GL_TRIANGLES, 0, mesh.positions.size() );

    // set state back
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glUseProgram( 0 );
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}