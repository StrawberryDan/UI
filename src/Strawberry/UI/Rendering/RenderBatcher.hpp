#pragma once
#include "Strawberry/Vulkan/Resource/Buffer.hpp"
#include "Strawberry/Core/Types/Variant.hpp"
#include "Strawberry/Vulkan/Pipeline/GraphicsPipeline.hpp"
#include <deque>
#include <map>

#include "Strawberry/Vulkan/Queue/CommandBuffer.hpp"

namespace Strawberry::UI
{
	class RenderBatcher
	{
	public:
		struct Batch
		{
			Batch() = default;
			Batch(const Batch&) = delete;
			Batch& operator=(const Batch&) = delete;
			Batch(Batch&& batch) = default;
			Batch& operator=(Batch&& batch) = default;

			unsigned int                                  vertexCount = 0;
			unsigned int                                  instanceCount = 1;

			template <typename T>
			class PtrOrVal : public Core::Variant<T, T*>
			{
			public:
				using Core::Variant<T, T*>::Variant;

				[[nodiscard]] const T& Resolve() const
				{
					return this->Visit(
						[](const T* v) -> const T& { return *v; },
						[](const T& v) -> const T& { return  v; }
					);
				}
			};

			Vulkan::GraphicsPipeline*                               pipeline;
			std::map<unsigned int, PtrOrVal<Vulkan::Buffer>>        vertexBuffers;
			Core::Optional<Vulkan::Buffer>                          indexBuffer;
			std::map<unsigned int, PtrOrVal<Vulkan::DescriptorSet>> descriptorSets;
		};


		void Render(Vulkan::CommandBuffer& commandBuffer)
		{
			Core::Optional<const Batch*> lastBatch;

			for (const auto& batch : mEntries)
			{
				TransitionQueueState(commandBuffer, batch, lastBatch);
				if (batch.indexBuffer)
				{
					commandBuffer.DrawIndexed(batch.vertexCount, batch.instanceCount);
				}
				else
				{
					commandBuffer.Draw(batch.vertexCount, batch.instanceCount);
				}

				lastBatch = &batch;
			}
		}


		void Append(Batch&& entry)
		{
			mEntries.emplace_back(std::move(entry));
		}


		template <std::ranges::range Range>
		void Append(Range&& entries)
		{
			mEntries.append_range(std::forward<Range>(entries));
		}


		bool Empty() const noexcept
		{
			return mEntries.empty();
		}


		void Clear()
		{
			mEntries.clear();
		}

	private:
		static void TransitionQueueState(Vulkan::CommandBuffer& commandBuffer, const Batch& currentBatch, const Core::Optional<const Batch*>& lastBatch)
		{
			if (currentBatch.pipeline != lastBatch.Map([] (const auto& x) { return x->pipeline; } ))
			{
				commandBuffer.BindPipeline(*currentBatch.pipeline);
			}


			for (const auto& [index, descriptorSet] : currentBatch.descriptorSets)
			{
				if (!lastBatch.HasValue() || (!lastBatch->descriptorSets.contains(index) || descriptorSet != lastBatch->descriptorSets.at(index)))
				{
					commandBuffer.BindDescriptorSet(*currentBatch.pipeline, index, descriptorSet.Resolve());
				}
			}

			for (const auto& [index, vertexBuffer] : currentBatch.vertexBuffers)
			{
				if (!lastBatch.HasValue() || (!lastBatch->vertexBuffers.contains(index) || vertexBuffer != lastBatch->vertexBuffers.at(index)))
				{
					commandBuffer.BindVertexBuffer(index, vertexBuffer.Resolve());
				}
			}
		}


	private:
		std::deque<Batch> mEntries;
	};
}
