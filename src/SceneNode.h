/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2017 Ultimaker b.v. <j.vankessel@ultimaker.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SAVITAR_SCENENODE_H
#define SAVITAR_SCENENODE_H

#include "SavitarExport.h"
#include "MeshData.h"

#include <string>
#include <vector>
#include <map> // For std::map

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
        void setTransformation(std::string);

        std::vector<SceneNode*> getChildren();
        std::vector<SceneNode*> getAllChildren();
        void addChild(SceneNode* node);

        MeshData& getMeshData();
        void setMeshData(MeshData mesh_data);

        /**
         * Set the data of this SceneNode by giving it a xml node
         */
        void fillByXMLNode(pugi::xml_node xml_node);

        /**
         * Get the (unique) identifier of the node.
         */
        std::string getId();

        void setId(std::string id);

        /**
         * Get the (per-object) settings attached to this SceneNode.
         * Note that this is part of the Cura Extension and not 3mf Core.
         */
        std::map<std::string, std::string> getSettings();

        void setSetting(std::string key, std::string value);

    protected:
        std::string transformation;
        std::vector<SceneNode*> children;
        MeshData mesh_data;
        std::map<std::string, std::string> settings;
        std::string id;
    };
}

#endif
