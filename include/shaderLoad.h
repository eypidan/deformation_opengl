
#ifndef SHADERLOAD_H
#define SHADERLOAD_H

class Shader {
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const char* geometryPath = nullptr);
    // use/activate the shader
    void use() {
        glUseProgram(ID);
    }
    // utility uniform functions
    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec2(const std::string& name, const Eigen::Vector2f& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()),1, value.data());
        //glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const Eigen::Vector3f& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1,value.data());
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const  Eigen::Vector4f& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1,  value.data());
        //glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const  Eigen::Matrix2f& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const Eigen::Matrix3f& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const Eigen::Matrix4f& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1,GL_FALSE,mat.data());
    }
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
private:
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif