/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.inl
@Time: 2021/6/1 10:13 下午
@Desc:
***************************/

#ifndef CGRAPH_GELEMENT_INL
#define CGRAPH_GELEMENT_INL

CGRAPH_NAMESPACE_BEGIN

template<typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int>>
CStatus GElement::createGParam(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(this->param_manager_)

    status = this->param_manager_->create<T>(key);
    CGRAPH_FUNCTION_END
}


template<typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int>>
T* GElement::getGParam(const std::string& key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->param_manager_)

    T* ptr = this->param_manager_->get<T>(key);
    return ptr;
}


template<typename TAspect, typename TParam,
        std::enable_if_t<std::is_base_of<GAspect, TAspect>::value, int>,
        std::enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int>>
GElementPtr GElement::addGAspect(TParam* param) {
    if (!aspect_manager_) {
        /** 采用懒加载的方式执行，这里不会有并发问题，故不需要采用单例模式了 */
        aspect_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GAspectManager)
    }

    GAspectPtr aspect = CGRAPH_SAFE_MALLOC_COBJECT(TAspect)
    aspect->setName(this->getName())
        ->setAParam<TParam>(param)
        ->setPipelineParamManager(this->param_manager_);
    aspect_manager_->add(aspect);
    return this;
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GELEMENT_INL
