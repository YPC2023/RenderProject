#pragma once

// 这里的 MYLIB_EXPORTS 是我们在 DLL 项目属性中定义的开关宏
#ifdef ENGINE_API_EXPORTS
    // 如果定义了 MYLIB_EXPORTS，说明正在编译 DLL，执行导出
#define ENGINE_API_ACTION __declspec(dllexport)
#else
    // 如果没有定义，说明是外部项目在使用该库，执行导入
#define ENGINE_API_ACTION __declspec(dllimport)
#endif