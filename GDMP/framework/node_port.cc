#include "GDMP/framework/graph_builder.h"
#include "GDMP/framework/graph_node.h"

MediaPipeNodeDestination *MediaPipeNodeDestination::_new(Ref<MediaPipeGraphBuilder> builder, int index) {
	MediaPipeNodeDestination *dst = MediaPipeNodeDestination::_new();
	ERR_FAIL_COND_V(builder.is_null(), dst);
	dst->builder = builder;
	dst->_get_dst = [dst, index]() { return dst->builder->get_builder().Out(index); };
	return dst;
}

MediaPipeNodeDestination *MediaPipeNodeDestination::_new(Ref<MediaPipeGraphBuilder> builder, String tag) {
	MediaPipeNodeDestination *dst = MediaPipeNodeDestination::_new();
	ERR_FAIL_COND_V(builder.is_null(), dst);
	dst->builder = builder;
	dst->_get_dst = [dst, tag]() { return dst->builder->get_builder().Out(tag.utf8().get_data()); };
	return dst;
}

MediaPipeNodeDestination *MediaPipeNodeDestination::_new(Ref<MediaPipeGraphNode> node, int index) {
	MediaPipeNodeDestination *dst = MediaPipeNodeDestination::_new();
	ERR_FAIL_COND_V(node.is_null(), dst);
	dst->node = node;
	dst->_get_dst = [dst, index]() { return dst->node->get_node()->In(index); };
	return dst;
}

MediaPipeNodeDestination *MediaPipeNodeDestination::_new(Ref<MediaPipeGraphNode> node, String tag) {
	MediaPipeNodeDestination *dst = MediaPipeNodeDestination::_new();
	ERR_FAIL_COND_V(node.is_null(), dst);
	dst->node = node;
	dst->_get_dst = [dst, tag]() { return dst->node->get_node()->In(tag.utf8().get_data()); };
	return dst;
}

bool MediaPipeNodeDestination::connect(builder::Source<> &src) {
	ERR_FAIL_NULL_V(_get_dst, false);
	src.ConnectTo(_get_dst());
	return true;
}

MediaPipeNodeSource *MediaPipeNodeSource::_new(Ref<MediaPipeGraphBuilder> builder, int index) {
	MediaPipeNodeSource *src = MediaPipeNodeSource::_new();
	ERR_FAIL_COND_V(builder.is_null(), src);
	src->builder = builder;
	src->_get_src = [src, index]() { return src->builder->get_builder().In(index); };
	return src;
}

MediaPipeNodeSource *MediaPipeNodeSource::_new(Ref<MediaPipeGraphBuilder> builder, String tag) {
	MediaPipeNodeSource *src = MediaPipeNodeSource::_new();
	ERR_FAIL_COND_V(builder.is_null(), src);
	src->builder = builder;
	src->_get_src = [src, tag]() { return src->builder->get_builder().In(tag.utf8().get_data()); };
	return src;
}

MediaPipeNodeSource *MediaPipeNodeSource::_new(Ref<MediaPipeGraphNode> node, int index) {
	MediaPipeNodeSource *src = MediaPipeNodeSource::_new();
	ERR_FAIL_COND_V(node.is_null(), src);
	src->node = node;
	src->_get_src = [src, index]() { return src->node->get_node()->Out(index); };
	return src;
}

MediaPipeNodeSource *MediaPipeNodeSource::_new(Ref<MediaPipeGraphNode> node, String tag) {
	MediaPipeNodeSource *src = MediaPipeNodeSource::_new();
	ERR_FAIL_COND_V(node.is_null(), src);
	src->node = node;
	src->_get_src = [src, tag]() { return src->node->get_node()->Out(tag.utf8().get_data()); };
	return src;
}

bool MediaPipeNodeSource::connect_to(Ref<MediaPipeNodeDestination> dst, String stream_name) {
	ERR_FAIL_NULL_V(_get_src, false);
	ERR_FAIL_COND_V(dst.is_null(), false);
	return dst->connect(_get_src().SetName(stream_name.utf8().get_data()));
}
