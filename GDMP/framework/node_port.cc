#include "GDMP/framework/graph_builder.h"
#include "GDMP/framework/graph_node.h"

MediaPipeNodeDestination::MediaPipeNodeDestination(Ref<MediaPipeGraphBuilder> builder, int index) {
	ERR_FAIL_NULL(builder);
	_get_dst = [builder, index]() { return builder->get_builder().Out(index); };
}

MediaPipeNodeDestination::MediaPipeNodeDestination(Ref<MediaPipeGraphBuilder> builder, const String &tag) {
	ERR_FAIL_NULL(builder);
	_get_dst = [builder, tag]() { return builder->get_builder().Out(tag.utf8().get_data()); };
}

MediaPipeNodeDestination::MediaPipeNodeDestination(Ref<MediaPipeGraphNode> node, int index) {
	ERR_FAIL_NULL(node);
	_get_dst = [node, index]() { return node->get_node()->In(index); };
}

MediaPipeNodeDestination::MediaPipeNodeDestination(Ref<MediaPipeGraphNode> node, const String &tag) {
	ERR_FAIL_NULL(node);
	_get_dst = [node, tag]() { return node->get_node()->In(tag.utf8().get_data()); };
}

bool MediaPipeNodeDestination::connect(builder::Source<> &src) {
	ERR_FAIL_NULL_V(_get_dst, false);
	src.ConnectTo(_get_dst());
	return true;
}

MediaPipeNodeSource::MediaPipeNodeSource(Ref<MediaPipeGraphBuilder> builder, int index) {
	ERR_FAIL_NULL(builder);
	_get_src = [builder, index]() { return builder->get_builder().In(index); };
}

MediaPipeNodeSource::MediaPipeNodeSource(Ref<MediaPipeGraphBuilder> builder, const String &tag) {
	ERR_FAIL_NULL(builder);
	_get_src = [builder, tag]() { return builder->get_builder().In(tag.utf8().get_data()); };
}

MediaPipeNodeSource::MediaPipeNodeSource(Ref<MediaPipeGraphNode> node, int index) {
	ERR_FAIL_NULL(node);
	_get_src = [node, index]() { return node->get_node()->Out(index); };
}

MediaPipeNodeSource::MediaPipeNodeSource(Ref<MediaPipeGraphNode> node, const String &tag) {
	ERR_FAIL_NULL(node);
	_get_src = [node, tag]() { return node->get_node()->Out(tag.utf8().get_data()); };
}

bool MediaPipeNodeSource::connect_to(Ref<MediaPipeNodeDestination> dst, const String &stream_name) {
	ERR_FAIL_NULL_V(_get_src, false);
	ERR_FAIL_NULL_V(dst, false);
	return dst->connect(_get_src().SetName(stream_name.utf8().get_data()));
}
