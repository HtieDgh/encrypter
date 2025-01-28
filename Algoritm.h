/*
   Copyright 2025 Htie digital

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once
#include"OutputStrategy.h"
#include"InputStrategy.h"
namespace encrypt {
	class Algorithm
	{
	protected:
		OutputStrategy* _of = nullptr;
		InputStrategy* _if=nullptr;
	public:								   
		virtual bool mode() const = 0;
		virtual void mode(bool isEnc)=0;
		virtual void run()=0;

		void setOutput(OutputStrategy*);
		void setInput(InputStrategy*);
		~Algorithm();
	};
}


