#include "CTexture.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

std::vector<CTexture*>	            CTexture::m_vec_Texture;
std::map<unsigned int, CTexture*>	CTexture::m_map_Texture;

CTexture::CTexture(const S_TEXTURE_CONFIG& Config)
{
    m_vec_Texture.push_back(this);

    m_Config = Config;
    if (CUtils::FileExists(Config.strPath.c_str())) {
        m_bOK = LoadFromFile();
    }
    else if (0 != Config.width && 0 != Config.height) {
        m_bOK = CreateEmptyTexture();
    }

    m_map_Texture[m_nID] = this;
}

bool CTexture::LoadFromFile()
{
    if (!CUtils::FileExists(m_Config.strPath.c_str())) {
        PRINTLOG("File is not exists(%s)", m_Config.strPath.c_str());
        return false;
    }
    cv::Mat image = cv::imread(m_Config.strPath.c_str());
    if (image.empty()) {
        PRINTLOG("Fail to load texture(%s)", m_Config.strPath.c_str());
        return false;
    }

    GLenum format = GL_RED;
    int channel = image.channels();
    m_Config.width = image.cols;
    m_Config.height = image.rows;

    if (3 == channel) {
        format = GL_RGB;
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    }
    else if (4 == channel) {
        format = GL_RGBA;
        cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
    }
    cv::flip(image, image, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &m_nID);
    glBindTexture(GL_TEXTURE_2D, m_nID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Config.width, m_Config.height, 0, format, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Config.TEX_GL_TEXTURE_WRAP_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Config.TEX_GL_TEXTURE_WRAP_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Config.TEX_GL_TEXTURE_MIN_FILTER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Config.TEX_GL_TEXTURE_MAG_FILTER);
    return true;
}

bool CTexture::CreateEmptyTexture()
{
    glGenTextures(1, &m_nID);
    glBindTexture(GL_TEXTURE_2D, m_nID);

    // 纹理参数设置
    glTexImage2D(GL_TEXTURE_2D, 0, m_Config.Format, m_Config.width, m_Config.height, 
        0, m_Config.Format, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Config.TEX_GL_TEXTURE_WRAP_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Config.TEX_GL_TEXTURE_WRAP_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Config.TEX_GL_TEXTURE_MIN_FILTER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Config.TEX_GL_TEXTURE_MAG_FILTER);
    return true;
}

void CTexture::Resize(int width, int height)
{
    m_Config.width = width;
    m_Config.height = height;
    // 更新颜色纹理
    glBindTexture(GL_TEXTURE_2D, m_nID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_Config.Format, 
        width, height, 0, m_Config.Format, GL_UNSIGNED_BYTE, NULL);
}

bool CTexture::SaveToFile(const std::string& strPath)
{
    // 1. 初始化 Mat 空间 (注意：OpenCV 默认 BGR，高度在前)
    cv::Mat image(m_Config.height, m_Config.width, CV_8UC3);

    // 2. 绑定纹理并读取数据
    glBindTexture(GL_TEXTURE_2D, m_nID);
    // 强制 1 字节对齐，防止某些分辨率下出现图像倾斜
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // 直接读取为 BGR 格式以适配 OpenCV
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);

    // 3. 垂直翻转图像（修正 OpenGL 的 Y 轴方向）
    cv::flip(image, image, 0);

    // 4. 使用 OpenCV 保存文件
    return cv::imwrite(strPath.c_str(), image);
}