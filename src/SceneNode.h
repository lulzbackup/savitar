/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2017 Ultimaker b.v. <j.vankessel@ultimaker.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SAVITAR_SCENENODE_H
#define SAVITAR_SCENENODE_H

#include "SavitarExport.h"
#include "MeshData.h"

#include <string>
#include <vector>

namespace pugi
{
    class xml_node;
}

namespace Savitar
{
    class SAVITAR_EXPORT SceneNode
    {
    public:
        SceneNode();
        virtual ~SceneNode();

        std::string getTransformation();
        void setStransformation(std::string);

        std::vector<SceneNode> getChildren();
        void addChild(SceneNode node);

        MeshData& getMeshData();
        void setMeshData(MeshData mesh_data);

        /**
         * Set the data of this SceneNode by giving it a xml node
         */
        void fillByXMLNode(pugi::xml_node xml_node);

        std::string getId();

    protected:
        std::string transformation;
        std::vector<SceneNode> children;
        MeshData mesh_data;

        std::string id;
    };
}

#endif