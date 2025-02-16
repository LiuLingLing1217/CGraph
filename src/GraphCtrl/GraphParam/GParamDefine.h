/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamDefine.h
@Time: 2022/1/25 10:34 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPARAMDEFINE_H
#define CGRAPH_GPARAMDEFINE_H

CGRAPH_NAMESPACE_BEGIN

#define CGRAPH_CREATE_GPARAM(Type, key)               \
    this->createGParam<Type>(key);                    \

#define CGRAPH_GET_GPARAM(Type, key)                  \
    this->getGParam<Type>(key);                       \

#define CGRAPH_PARAM_WRITE_CODE_BLOCK(param)                                    \
    CGraph::CGRAPH_WRITE_LOCK __paramWLock__((param)->_param_shared_lock_);     \

#define CGRAPH_PARAM_READ_CODE_BLOCK(param)                                     \
    CGraph::CGRAPH_READ_LOCK __paramRLock__((param)->_param_shared_lock_);      \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPARAMDEFINE_H
