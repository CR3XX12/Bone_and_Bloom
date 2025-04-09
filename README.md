# 🌹Bone and Bloom🌱
**Bone and Bloom** is a stylized third-person multiplayer RPG prototype developed using **Unreal Engine 5**, blending **C++ and Blueprints** to implement core gameplay systems. The project introduces AI enemies, XP leveling, interactable UI, and networked multiplayer systems, all wrapped in a lighthearted aesthetic.

The game features Enemy's Health and AI, experience-based leveling, NPC interactions, animated doors, and full multiplayer replication, ideal for demonstrating both gameplay design and networking fundamentals.

![image](https://github.com/user-attachments/assets/70aa9ee2-3f0d-4b64-845c-5c9b345d1dbd)

## ⚔️ Gameplay Features

### 🧱 **Simple UI Elements**
  - XP counter, level-up indicator, interaction text.
  - Clean and minimal interface to maintain stylized feel.

![image](https://github.com/user-attachments/assets/bc20e703-d71b-441d-ac12-834c5854f65e)

### 🗣️ **NPC Interaction**
  - Approach NPCs to trigger interaction prompts.
  - Designed to extend dialogue or quest-giving in the future.

### 🧠 **Experience & Leveling System**
  - Each coin pickup (from kills) adds XP.
  - UI updates in real time and displays **XP count** and **level-up notification**.

![image](https://github.com/user-attachments/assets/9d215cab-6e56-4773-95d1-57fb5ede0326)

### 🚪 **Animated Doors**
  - Fully animated doors that open when interacted with.
  - Uses networked RPCs to keep door states consistent in multiplayer.

![image](https://github.com/user-attachments/assets/47427816-0dc3-4482-b208-44f3fa3c66a6)

### 👹 **Enemy AI & Combat**
  - Enemies detect the player via **sight** and **sound**.
  - Defeating skeletons and bosses grants **XP** and **collectible coins**.

![image](https://github.com/user-attachments/assets/e86aa6c6-3123-41e1-91b8-4b2396073076)

## 🌐 Multiplayer Networking
Built with multiplayer in mind, the game follows core networking principles:

- Actor replication and ownership management.
- Use of **Remote Procedure Calls (RPCs)** for interaction syncing.
- AI behavior adapted for multiplayer environments.
- Replicated UI events (like XP gain, level-ups) visible to clients.

![image](https://github.com/user-attachments/assets/5d302658-1fdd-4d7f-bc3e-f2a773981d53)

## 🛠️ Built With

- **Unreal Engine 5**
- **C++ and Blueprints Hybrid Project**
- **Multiplayer Logic Implemented with Dedicated Server Preview**
- **AI Perception & Behavior Trees**
