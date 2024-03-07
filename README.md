# GCInventorySystem
 A generic inventory system that supports replication. It's an experimental plugin use it at your own risk.

# Important notes:
 - First, you must add the plugin to your unreal engine project.
 - All the items are going to be identified by gameplay tags logic-wise which means that every item you create should have a unique gameplay tag that identifies it.
 - The inventory system comprises multiple classes: An inventory Actor component, an Inventory GIS subsystem, an inventory Interface, and the Inventory Mapping data asset.

# Setting up the inventory system
 - Every actor holding an inventory should implement the **GCInventoryInterface** interface class and have a **GCInventoryActorComponent** as a component for the actor.
 - All the items used in the game should be defined in a data asset that inherits from **UGCInventoryMappingDataAsset**. Here there are two possible maps. One defines all the items in the game separated by categories and the other map holds the recipes for the items that you wish to craft.
 - The ItemsCategoryMap in the **UGCInventoryMappingDataAsset** is a map that holds the reference to all the items in the game. They're separated by category so you can separate your items in different categories for easier management. Each category holds a data table that could be any struct that inherits from **FTableRowBase**. So it should be generic enough for it to be used with any kind of information you need.
 - The only restriction for the data table that holds the item's information is the row name. **The row name should be the gameplay tag used to identify the item.**
 - To set up your inventory data asset you must add these lines inside your DefaultGame.ini. Keep in mind that **StructDataAsset=** asks for the path of the data asset file.
   ```
   [/Script/GCInventorySystem.GCInventoryGISSubsystems]
   StructDataAsset=/Game/DT_InventoryDataAsset.DT_InventoryDataAsset (***example path***)
   ```
