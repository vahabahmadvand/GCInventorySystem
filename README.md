# GCInventorySystem
 A generic inventory system that supports replication. It's an experimental plugin use it at your own risk.

# Important notes:
 - First, you must add the plugin to your unreal engine project.
 - All the items are going to be identified by gameplay tags logic-wise. This means that every item you create should have a unique gameplay tag that identifies it.
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
# Inventory Actor Component
- This is an actor component that is in charge of holding the inventory of the actor.
- Check the inventory methods of this class to operate it: AddItemToInventory, UseItemFromInventory, DropItemFromInventory, RemoveItemFromInventory. And some more.
- All of these methods use the gameplay tag of the item to use them. So as long as you have the tag of the item, you should be able to operate the inventory without issues.
- As an important note, there are two methods in the inventory **RemoveItemFromInventory** and **DropItemFromInventory**. Even tho logic-wise could be the same, I decided to make a method for each of them. Both of them would remove the item from the inventory of the owner. But you can extend them differently if needed. Please keep in mind that they are independent of each other so calling drop item won't call remove item or vice-versa.
- This component has some delegates for the following events: OnItemGranted, OnItemUsed, and OnItemRemoved. Use them if needed.

# Inventory Interface
- The **GCInventoryInterface** class should be implemented in the actor that holds the inventory component. The reason is that this class possesses some methods to extend the functionality of the inventory as needed.
- When you operate items in your inventory component you operate using only gameplay tags. But internally the **UGCInventoryGISSubsystems** will return to the interface of the item holder all the item information from the data table.
- In the end, you'll receive a data table that you can cast to your struct and get all the information that you need from there and implement the logic you need.

# Crafting teaser
- There's a little bit of logic for a crafting system in this plugin. It is really basic, and again use it at your own risk.
- In the **UGCInventoryMappingDataAsset** there's a map called **ItemsCategoryCraftingRecipes**. Similar to the other map. Here you can separate all the recipes by category.
  ```
  TMap<FGameplayTag, FItemRecipeInfo> ItemsCategoryCraftingRecipes;
  ```
- Each category will be identified by a gameplay tag and the **FItemRecipeInfo** struct is composed of another map called **ItemRecipes**
  ```
  TMap<FGameplayTag, FItemRecipeElements> ItemRecipes;
  ```
- Each element of the **ItemRecipes** map is an item (identified by its tag) and **FItemRecipeElements** struct. That as you guessed, contains another map (XD) called **RecipeElements**.
  ```
  TMap<FGameplayTag, float> RecipeElements;
  ```
- Each element of this map is the items and quantity of them needed to craft an item.
- Finally, once you defined all your recipes, you can execute the method **CraftItem** in the **GCInventoryActorComponent**. This method will take as input an item tag, and it'll check if you possess the required items for the recipe. And if you do, the materials will be removed from your inventory and the crafted item will be added to your inventory.

That's all for now. I hope it works for you all and good luck.
PD. If this crashes your game, you were warned in the beginning 
 ```
¯\_(ツ)_/¯
```
